#include "header.h"
#include "types.h"

bool sign_of_tan(float angle) { return (int)(angle / PI_2) & 1; }  // FIXME not using, should be removed or used

#define DEBUG

int round_upper(float f) {  // default
  if (isinf(f)) {
    if (f > 0) return INT_MAX;
    else return INT_MIN;
  } else {
    return (int)roundf(f + EPSILON);
  }
}

int round_lower(float f) {
  if (isinf(f)) {
    if (f > 0) return INT_MAX;
    else return INT_MIN;
  } else {
    return (int)roundf(f - EPSILON);
  }
}

void scan(Map map, FloatPair pos, float dir) {
#ifdef DEBUG
  char **sight = malloc((unsigned)map.row * sizeof(char *));
  for (int i = 0; i < map.row; i++) sight[i] = memset(malloc((unsigned)map.col * sizeof(char)), '-', (unsigned)map.col);
#endif
  float angle = dir * PI_2, l_angle = fmodf(angle + fov / 2 + 2 * PI + EPSILON, 2 * PI), r_angle = fmodf(angle - fov / 2 + 2 * PI, 2 * PI);
  float l_tan = tanf(-l_angle), r_tan = tanf(-r_angle);
#ifdef DEBUG
  printf("angle   = %f (%f pi)\n", angle, angle / PI);
  printf("l_angle = %f (%f pi)\tl_tan = %f\n", l_angle, l_angle / PI, l_tan);
  printf("r_angle = %f (%f pi)\tr_tan = %f\n", r_angle, r_angle / PI, r_tan);
  printf("\n");
#endif
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  float l_y, r_y;
  // the y coordinate of the intersection point of l/r_line and the l/r_bound
  if (l_angle > PI_2 && l_angle <= 3 * PI_2) l_y = l_tan * ((-.5f) - pos.first) + pos.second;  // l_bound
  else l_y = l_tan * (((float)map.col - .5f) - pos.first) + pos.second;                        // r_bound
  if (r_angle > PI_2 && r_angle <= 3 * PI_2) r_y = r_tan * ((-.5f) - pos.first) + pos.second;  // l_bound
  else r_y = r_tan * (((float)map.col - .5f) - pos.first) + pos.second;                        // r_bound
#ifdef DEBUG
  printf("l_y = %f, r_y = %f\n", l_y, r_y);
#endif
  int upper_row, lower_row;
  if (l_y < r_y) upper_row = round_lower(l_y), lower_row = round_upper(r_y);
  else upper_row = round_lower(r_y), lower_row = round_upper(l_y);
  int mid = round_upper(pos.second), row_begin = max(min(min(upper_row, lower_row), mid), 0), row_end = min(max(max(upper_row, lower_row), mid), map.row - 1);
#ifdef DEBUG
  printf("upper_row = %d\t, lower_row = %d\n\n", upper_row, lower_row);
  printf("row_begin = %d, mid = %d, row_end = %d\n", row_begin, mid, row_end);
#endif
  for (int row = row_begin; row <= row_end; row++) {
    int col_begin, col_end;  // the leftmost/rightmost block's col of the l/r_line when y = row
    if (row <= mid - (angle > PI ? 1 : 0)) col_begin = l_angle > PI ? 0 : max(round_lower(((float)row + (l_angle < PI_2 ? .5f : -.5f) - pos.second) / l_tan + pos.first), 0), col_end = r_angle > PI ? map.col - 1 : min(round_upper(((float)row + (r_angle < PI_2 ? -.5f : .5f) - pos.second) / r_tan + pos.first), map.col - 1);
    else col_begin = r_angle < PI ? 0 : max(round_lower(((float)row + (r_angle < 3 * PI_2 ? .5f : -.5f) - pos.second) / r_tan + pos.first), 0), col_end = l_angle < PI ? map.col - 1 : min(round_upper(((float)row + (l_angle < 3 * PI_2 ? -.5f : .5f) - pos.second) / l_tan + pos.first), map.col - 1);
#ifdef DEBUG
    // printf("row = %d, col_begin = %d, col_end = %d\n", row, col_begin, col_end);
    if (row == 5) printf("%s  row = %d, col_begin = %d, col_end = %d\n", row <= mid - (angle > PI ? 1 : 0) ? "--upper--" : "--lower--", row, col_begin, col_end);
#endif
    for (int col = col_begin; col <= col_end; col++) {
      if (map.data[row][col] == '@') {
#ifdef DEBUG
        sight[row][col] = '%';
#endif
        if (!float_equal((float)row, pos.second)) {
          for (float r = (float)row - .5f; r <= (float)row + .5f; r += render_spacing) {  // TODO change to dinamic spacing
            for (float h = -0.5; h <= 0.5; h += render_spacing) {
              ////////////////////////////
              // zb = 1 / sqrt(pow(x, 2) + pow(z, 2)),                           //
              //     xp = (int)((float)col / 2 + x * scaling_factor / z + 0.5),  //
              //     yp = (int)((float)row / 2 + (y * scaling_factor / z) / 2 + 0.5);
              // // printf("(%6.3f, %6.3f, %6.3f) -> (%2d, %2d)\t%f\n", x, y, z, xp, yp, luminance(normal, x, y, z, zb));
              // // printf("%f %f %f (%d, %d)\n", x, y, z, xp, yp);
              // if ((xp >= 0 && xp < col) && (yp >= 0 && yp < row) && zb > zbuffer[xp][yp]) zbuffer[xp][yp] = zb, output[xp][yp] = ".,-~:;=!*#$@"[(int)luminance(normal, x, y, z)];
            }
          }
        }
      }

      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
  }
#ifdef DEBUG

  printf(" _ |");
  for (int i = 0; i < map.col; i++) printf("%2d|", i);

  for (int i = 0; i < map.row; i++) {
    printf("\n%2d |", i);
    for (int j = 0; j < map.col; j++) {
      printf("%s%c ", j == pos.first && i == pos.second ? " \e[93mP\e[0" : " ", j == pos.first && i == pos.second ? 'm' : sight[i][j]);
    }
  }
  printf("\n");

  for (int i = 0; i < map.row; i++) free(sight[i]);
  free(sight);
#endif
}

int main() {
  int ii;
  //   float max_dir = 4 * PI;
  Map *map = new_Map(11, 17);
  float dir;
  for (int i = 0; i < 10; i++) printf("\n");
  for (dir = 0.f; dir <= 12.f / 3.f; dir += rotate_spacing) {
    printf("\n\n\nCalling scan() with angle = %f * PI/2 => %f\n\n", dir, dir * PI_2);
    scan(*map, make_FloatPair(8.f, 5.f), fmodf(dir, 4.f));
    delay(1.f);
    // printf("\e[1;1H\e[2J");
  }

  //   new_tan(0, -5.f / 2.f * PI);
  //   printf("%g", 1.f / tanf(PI / 2));
  Map_clear(map);
  // for (float angle = 0; angle < 2 * PI + 0.1; angle += PI / 16) {
  //   printf("%-8g\t(%g\tpi):\t%d\n", angle, angle / PI, sign_of_tan(angle));
  // }
}