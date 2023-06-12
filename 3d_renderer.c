#include "header.h"
#include "types.h"

// #define DEBUG
// #define ROTATE

// int luminance_index(float normal[3], float light_vector[3]) { return -(normal[0] * light_vector[0] + normal[1] * light_vector[1] + normal[2] * light_vector[2]) * 11.25f * powf(powf(light_vector[0], 2.f) + powf(light_vector[1], 2.f) + powf(light_vector[2], 2.f), -.5f); }
int luminance_index(float normal[], float x, float y, float z) { return (int)(-11.25f * (normal[0] * x + normal[1] * y + normal[2] * z) * powf(powf(x, 2) + powf(y, 2) + powf(z, 2), -0.5) / (1.f + fabsf(z))); }

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

void render(Map map, FloatPair pos, float dir) {
  float **z_buffer = malloc((unsigned)win_row * sizeof(float *));
  char **display = malloc((unsigned)win_row * sizeof(char *));
  for (int i = 0; i < win_row; i++) z_buffer[i] = calloc((unsigned)win_col, sizeof(float)), display[i] = memset(malloc((unsigned)win_col * sizeof(char)), ' ', (unsigned)win_col * sizeof(char));

  float angle = dir * PI_2, l_angle = fmodf(angle + fov / 2 + 2 * PI + EPSILON, 2 * PI), r_angle = fmodf(angle - fov / 2 + 2 * PI, 2 * PI);

  float l_tan = tanf(-l_angle), r_tan = tanf(-r_angle);
  float dir_sin = sinf(-angle), dir_cos = cosf(-angle);
  FloatPair parallel_spacing = make_FloatPair(sinf(angle) * render_spacing, cosf(angle) * render_spacing),  //
      orthogonal_spacing = make_FloatPair(parallel_spacing.second, -parallel_spacing.first);                // TODO remove one
#ifdef DEBUG
  char **sight = malloc((unsigned)map.row * sizeof(char *));
  for (int i = 0; i < map.row; i++) sight[i] = memset(malloc((unsigned)map.col * sizeof(char)), '-', (unsigned)map.col * sizeof(char));
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

  int upper_row, lower_row;
  if (l_y < r_y) upper_row = round_lower(l_y), lower_row = round_upper(r_y);
  else upper_row = round_lower(r_y), lower_row = round_upper(l_y);
  int mid = round_upper(pos.second), row_begin = max(min(min(upper_row, lower_row), mid), 0), row_end = min(max(max(upper_row, lower_row), mid), map.row - 1);
#ifdef DEBUG
  printf("l_y = %f, r_y = %f\n\n", l_y, r_y);
  printf("upper_row = %d\t, lower_row = %d\n\n", upper_row, lower_row);
  printf("row_begin = %d, mid = %d, row_end = %d\n\n", row_begin, mid, row_end);
#endif
  float abs_x, abs_z, x_end, z_end, abs_normal;
  float x, z, normal[3] = {};
  for (int row = row_begin; row <= row_end; row++) {
    int col_begin, col_end;  // the leftmost/rightmost block's col of the l/r_line when y = row
    if (row <= mid - (angle > PI ? 1 : 0)) col_begin = l_angle > PI ? 0 : max(round_lower(((float)row + (l_angle < PI_2 ? .5f : -.5f) - pos.second) / l_tan + pos.first), 0), col_end = r_angle > PI ? map.col - 1 : min(round_upper(((float)row + (r_angle < PI_2 ? -.5f : .5f) - pos.second) / r_tan + pos.first), map.col - 1);
    else col_begin = r_angle < PI ? 0 : max(round_lower(((float)row + (r_angle < 3 * PI_2 ? .5f : -.5f) - pos.second) / r_tan + pos.first), 0), col_end = l_angle < PI ? map.col - 1 : min(round_upper(((float)row + (l_angle < 3 * PI_2 ? -.5f : .5f) - pos.second) / l_tan + pos.first), map.col - 1);
#ifdef DEBUG
      // printf("row = %d, col_begin = %d, col_end = %d\n", row, col_begin, col_end);
      // if (row == 5) printf("%s  row = %d, col_begin = %d, col_end = %d\n", row <= mid - (angle > PI ? 1 : 0) ? "--upper--" : "--lower--", row, col_begin, col_end);
#endif
    for (int col = col_begin; col <= col_end; col++) {
      if (map.data[row][col] == '@') {
#ifdef DEBUG
        sight[row][col] = '%';
#endif

        //              .
        //             /|\ Y
        //              |      __. Z
        //              |       /|
        //              |     /
        //              |   /
        //              | /
        // -------------+-------------> X
        //            / |
        //          /   |
        //        /     |
        //      /       |
        //              |
        //              |

        if (!float_equal(row, pos.second)) {
#ifdef DEBUG
          printf("print the %s side of block (%d, %d)\n", row > pos.second ? "north" : "south", row, col);
#endif
          abs_x = (float)row - pos.second + (row > pos.second ? -.5f : .5f), abs_z = (float)col - pos.first - .5f, x_end = abs_x * dir_cos - (abs_z + 1) * dir_sin, z_end = abs_x * dir_sin + (abs_z + 1) * dir_cos, abs_normal = row > pos.second ? -1 : 1;

          x = abs_x * dir_cos - abs_z * dir_sin, z = abs_x * dir_sin + abs_z * dir_cos, normal[0] = abs_normal * dir_cos, normal[2] = abs_normal * dir_sin;
#ifdef DEBUG
          printf("abs_x = %f, abs_z = %f, abs_normal = %f\n", abs_x, abs_z, abs_normal);
          printf("x_begin = %f, z_begin =%f, x_end = %f, z_end = %f\n", x, z, x_end, z_end);
          for (int i = 0; i < 3; i++) printf("normal[%d] = %f ", i, normal[i]);
          printf("\n");
#endif
          for (bool x_increase = x_end > x, z_increase = z_end > z; ((x_increase && x <= x_end) || (!x_increase && x >= x_end)) && ((z_increase && z <= z_end) || (!z_increase && z >= z_end)); x += parallel_spacing.first * (1.f + z * z), z += parallel_spacing.second * (1.f + z * z)) {
            for (float y = -wall_height_2; y <= wall_height_2; y += render_spacing * (1.f + z * z)) {
              if (z < 0) continue;
              float zb = 1.f / z;
              int win_x = round_upper((float)win_col / 2 + x * scaling_factor / z + .5f), win_y = round_upper((float)win_row / 2 + (y * scaling_factor / z) / 2 + .5f);

              // printf("(x = %f, z = %f) => zb = %f, win_x = %d, win_y = %d, render_spacing = %f\n", x, z, zb, win_x, win_y, render_spacing * (1.f + z * z));

              if ((win_y >= 0 && win_y < win_row) && (win_x >= 0 && win_x < win_col) && zb > z_buffer[win_y][win_x]) z_buffer[win_y][win_x] = zb, display[win_y][win_x] = grayscale[luminance_index(normal, x, y, z)];
            }
          }
        }

        if (!float_equal(col, pos.first)) {
#ifdef DEBUG
          printf("print the %s side of block (%d, %d)\n", col > pos.first ? "west" : "east", row, col);
#endif
          abs_x = (float)row - pos.second - .5f, abs_z = (float)col - pos.first + (col > pos.first ? -.5f : .5f), x_end = (abs_x + 1) * dir_cos - abs_z * dir_sin, z_end = (abs_x + 1) * dir_sin + abs_z * dir_cos, abs_normal = col > pos.first ? -1 : 1;

          x = abs_x * dir_cos - abs_z * dir_sin, z = abs_x * dir_sin + abs_z * dir_cos, normal[0] = abs_normal * -dir_sin, normal[2] = abs_normal * dir_cos;
#ifdef DEBUG
          printf("abs_x = %f, abs_z = %f, abs_normal = %f\n", abs_x, abs_z, abs_normal);
          printf("x_begin = %f, z_begin =%f, x_end = %f, z_end = %f\n", x, z, x_end, z_end);
          for (int i = 0; i < 3; i++) printf("normal[%d] = %f ", i, normal[i]);
          printf("\n");
#endif
          for (bool x_increase = x_end > x, z_increase = z_end > z; ((x_increase && x <= x_end) || (!x_increase && x >= x_end)) && ((z_increase && z <= z_end) || (!z_increase && z >= z_end)); x += orthogonal_spacing.first * (1.f + z * z), z += orthogonal_spacing.second * (1.f + z * z)) {
            for (float y = -wall_height_2; y <= wall_height_2; y += render_spacing * (1.f + z * z)) {
              if (z < 0) continue;
              float zb = 1.f / z;
              int win_x = round_upper((float)win_col / 2 + x * scaling_factor / z + .5f), win_y = round_upper((float)win_row / 2 + (y * scaling_factor / z) / 2 + .5f);

              // printf("(x = %f, z = %f) => zb = %f, win_x = %d, win_y = %d\n", x, z, zb, win_x, win_y);

              if ((win_y >= 0 && win_y < win_row) && (win_x >= 0 && win_x < win_col) && zb > z_buffer[win_y][win_x]) z_buffer[win_y][win_x] = zb, display[win_y][win_x] = grayscale[luminance_index(normal, x, y, z)];
            }
          }
        }
      }
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
  // printf("%d %d\n", win_row, win_col);
  for (int i = 0; i < win_row; i++) {
    for (int j = 0; j < win_col; j++) printf("%c", display[i][j]);
    printf("\n");
  }
  for (int i = 0; i < win_row; i++) {
    free(display[i]);
    free(z_buffer[i]);
  }
  free(display);
  free(z_buffer);
}

// int main() {
// #ifdef __linux__
//   struct winsize w;
//   ioctl(0, TIOCGWINSZ, &w);
//   win_row = w.ws_row, win_col = w.ws_col;
// #elif _WIN32
//   CONSOLE_SCREEN_BUFFER_INFO csbi;
//   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
//   win_row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1, win_col = csbi.srWindow.Right - csbi.srWindow.Left + 1;
// #endif

//   printf("win_row = %d, win_col = %d\n", win_row, win_col);
//   delay(1.f);

//   int ii;
//   //   float max_dir = 4 * PI;
//   Map *map = new_Map(35, 87);
//   for (int i = 0; i < map->row; i++) memset(map->data[i], '9', (unsigned)map->col);
//   map->data[4][8] = '@';
//   map->data[6][8] = '@';
//   map->data[4][7] = '@';

//   map->data[5][10] = '@';
//   map->data[5][6] = '@';
//   map->data[7][8] = '@';
//   map->data[3][8] = '@';

//   float dir;
// #ifdef ROTATE
//   for (int i = 0; i < 10; i++) printf("\n");
//   for (dir = 3.f; dir <= 55.f; dir += rotate_spacing) {  // 12.f / 3.f
//   #ifdef DEBUG
//     printf("\n\n\nCalling render() with angle = %f * PI/2 => %f\n\n", dir, dir * PI_2);
//   #endif
//     render(*map, make_FloatPair(8.f, 5.f), fmodf(dir, 4.f));
//     delay(.5f);
//     // getchar();
//     // printf("\e[1;1H\e[2J");
//   }
// #else
//   render(*map, make_FloatPair(8.f, 5.f), 2);
// #endif
//   //   new_tan(0, -5.f / 2.f * PI);
//   //   printf("%g", 1.f / tanf(PI / 2));
//   Map_clear(map);
//   // for (float angle = 0; angle < 2 * PI + 0.1; angle += PI / 16) {
//   //   printf("%-8g\t(%g\tpi):\t%d\n", angle, angle / PI, sign_of_tan(angle));
//   // }
// }