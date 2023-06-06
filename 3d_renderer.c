#include "header.h"
#include "types.h"

bool sign_of_tan(float angle) { return (int)(angle / PI_2) & 1; }  // FIXME not using, should be removed or used

int half_floor(float f) { return (int)floorf(f + .5f); }
int half_ceil(float f) { return (int)ceilf(f - .5f); }

void scan(Map map, FloatPair pos, float dir) {
  char **sight = malloc((unsigned)map.row * sizeof(char *));
  for (int i = 0; i < map.row; i++) sight[i] = memset(malloc((unsigned)map.col * sizeof(char)), '-', (unsigned)map.col);
  // dir = 1.14 / PI_2;

  float angle = dir * PI_2, l_angle = fmodf(angle + fov / 2 + 2 * PI + epsilon, 2 * PI), r_angle = fmodf(angle - fov / 2 + 2 * PI, 2 * PI);
  float l_tan = tanf(-l_angle), r_tan = tanf(-r_angle);

  // printf("%f %f\n", l_angle, 2 * PI);
  printf("angle   = %f (%f pi)\n", angle, angle / PI);
  printf("l_angle = %f (%f pi)\tl_tan = %f\n", l_angle, l_angle / PI, l_tan);
  printf("r_angle = %f (%f pi)\tr_tan = %f\n", r_angle, r_angle / PI, r_tan);
  printf("\n");
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  float l_y, r_y;
  // the y coordinate of the intersection point of l_line and the l/r_bound // BUG check if <=3/2 pi is necessary
  if (l_angle > PI_2 && l_angle <= 3 * PI_2) l_y = l_tan * ((-.5f) - pos.first) + pos.second;  // l_bound
  else l_y = l_tan * (((float)map.col - .5f) - pos.first) + pos.second;                        // r_bound

  if (r_angle > PI_2 && r_angle <= 3 * PI_2) r_y = r_tan * ((-.5f) - pos.first) + pos.second;  // l_bound
  else r_y = r_tan * (((float)map.col - .5f) - pos.first) + pos.second;                        // r_bound

  printf("l_y         = %f\t, r_y         = %f\n", l_y, r_y);

  int rounded_l_y = half_floor(l_y), rounded_r_y = half_ceil(r_y);

  // printf("rounded_l_y = %-*d, rounded_r_y = %d\n", 9 + (l_y < 0), rounded_l_y, rounded_r_y);
  printf("rounded_l_y = %d\t, rounded_r_y = %d\n", rounded_l_y, rounded_r_y);

  printf("\n");

  int row_begin = max(min(min(rounded_l_y, rounded_r_y), floorf(pos.second)), 0), row_end = min(max(max(rounded_l_y, rounded_r_y), ceilf(pos.second)), map.row - 1);
  // row_begin =
  // if (l_tan > 0) {
  //   row_begin = half_floor(l_y);       // TODO poteintial bug when fov is small (<pi/2)
  //   if (row_begin < 0) row_begin = 0;  // TODO remove this line (for debug only
  // } else {
  //   row_end = half_floor(l_y);                      // TODO poteintial bug when fov is small (<pi/2)
  //   if (row_end >= map.row) row_end = map.row - 1;  // TODO remove this line (for debug only
  // }

  printf("row_begin = %d, row_end = %d\n", row_begin, row_end);

  for (int row = row_begin; row <= row_end; row++) {
    //

    float l_x, r_x;  // TODO remove l_x and r_x

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // the x coordinate of the l_line when y = row
    if (l_angle < PI_2) l_x = ((float)row - (pos.second - .5f)) / l_tan + pos.first;
    else l_x = ((float)row - (pos.second + .5f)) / l_tan + pos.first;

    // the x coordinate of the r_line when y = row
    if (r_angle < PI_2) r_x = ((float)row - (pos.second + .5f)) / r_tan + pos.first;
    else r_x = ((float)row - (pos.second - .5f)) / r_tan + pos.first;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int col_begin = (int)roundf(l_x - epsilon);
    // fix when out of bound or l_angle > pi
    if (col_begin < 0 || l_angle > PI) col_begin = 0;
    else if (col_begin >= map.col) col_begin = map.col;

    int col_end = r_angle > PI ? map.col - 1 : (int)roundf(r_x);
    printf("row = %d, col_begin = %f => %d, col_end = %f => %d\n", row, l_x, col_begin, r_x, col_end);

    for (int col = col_begin; col <= (col_end >= map.col ? map.col - 1 : col_end); col++) {
      if (map.data[row][col] == '@') {
        sight[row][col] = '%';
      } else {
        printf("%c", map.data[row][col]);
      }
    }

    //
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // for () {
  // }

  printf(" _ |");
  for (int i = 0; i < map.col; i++) printf("%2d|", i);

  for (int i = 0; i < map.row; i++) {
    printf("\n%2d |", i);
    for (int j = 0; j < map.col; j++) {
      printf("%s%c ", j == pos.first && i == pos.second ? " \e[93mP\e[0" : " ", j == pos.first && i == pos.second ? 'm' : sight[i][j]);
    }
  }
  printf("\n");

  //   for (int i = -map.row / 2; i < map.row / 2; i++) {
  //     for (int j = -map.col / 2; j < map.col / 2; j++) {
  //       //

  //       if (new_tan(i, j, dir - fov / 2 - PI_2) && new_tan(i, j, dir + fov / 2 - PI_2)) {
  //         printf("0 ");
  //       } else {
  //         printf("  ");
  //       }
  //     }
  //     printf("\n");
  //   }

  // int row_begin=

  //   // if (k == 0) sleep(1000 * 1000);
  //   usleep(30 * 1000);
  //   printf("\e[1;1H\e[2J");

  for (int i = 0; i < map.row; i++) free(sight[i]);
  free(sight);
}

int main() {
  int ii;
  //   float max_dir = 4 * PI;
  Map *map = new_Map(11, 17);
  float dir;
  for (int i = 0; i < 10; i++) printf("\n");
  for (dir = 0.f; dir <= 12.f / 3.f; dir += rotate_spacing) {
    printf("\n\n\nCalling scan() with angle = %f * PI/2 => %f\n\n", dir, dir * PI_2);
    scan(*map, make_FloatPair(8, 5), fmodf(dir, 4.f));
    delay(1);
    // printf("\e[1;1H\e[2J");
  }

  //   new_tan(0, -5.f / 2.f * PI);
  //   printf("%g", 1.f / tanf(PI / 2));
  Map_clear(map);
  // for (float angle = 0; angle < 2 * PI + 0.1; angle += PI / 16) {
  //   printf("%-8g\t(%g\tpi):\t%d\n", angle, angle / PI, sign_of_tan(angle));
  // }
}