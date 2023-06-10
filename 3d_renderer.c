#include "header.h"
#include "types.h"

// #define DEBUG

// int luminance_index(float normal[3], float light_vector[3]) { return -(normal[0] * light_vector[0] + normal[1] * light_vector[1] + normal[2] * light_vector[2]) * 11.25f * powf(powf(light_vector[0], 2.f) + powf(light_vector[1], 2.f) + powf(light_vector[2], 2.f), -.5f); }
int luminance_index(float normal[], float x, float y, float z) {
  return 50;
  // printf("%d\n", (int)(-(normal[0] * x + normal[1] * y + normal[2] * z) * 11.25 * powf(powf(x, 2) + powf(y, 2) + powf(z, 2), -0.5)));
  // return (int)(-(normal[0] * x + normal[1] * y + normal[2] * z) * 11.25 * powf(powf(x, 2) + powf(y, 2) + powf(z, 2), -0.5));
}

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
  float **z_buffer = malloc((unsigned)win_row * sizeof(float *));
  char **display = malloc((unsigned)win_row * sizeof(char *));
  for (int i = 0; i < win_row; i++) z_buffer[i] = memset(malloc((unsigned)win_col * sizeof(float)), -INFINITY, (unsigned)win_col * sizeof(float)), display[i] = memset(malloc((unsigned)win_col * sizeof(char)), ' ', (unsigned)win_col * sizeof(char));

#ifdef DEBUG
  char **sight = malloc((unsigned)map.row * sizeof(char *));
  for (int i = 0; i < map.row; i++) sight[i] = memset(malloc((unsigned)map.col * sizeof(char)), '-', (unsigned)map.col * sizeof(char));
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

  int upper_row, lower_row;
  if (l_y < r_y) upper_row = round_lower(l_y), lower_row = round_upper(r_y);
  else upper_row = round_lower(r_y), lower_row = round_upper(l_y);
  int mid = round_upper(pos.second), row_begin = max(min(min(upper_row, lower_row), mid), 0), row_end = min(max(max(upper_row, lower_row), mid), map.row - 1);
#ifdef DEBUG
  printf("l_y = %f, r_y = %f\n\n", l_y, r_y);
  printf("upper_row = %d\t, lower_row = %d\n\n", upper_row, lower_row);
  printf("row_begin = %d, mid = %d, row_end = %d\n\n", row_begin, mid, row_end);
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
        //              .
        //             /|\ Y                                  |
        //              |      __. Z                          |      __. Z
        //              |       /|                            |       /|
        //              |     /                               |     /
        //              |   /                                 |   /
        //              | /                                   | /
        // -------------+-------------> X   =>   -------------+-------------> X
        //            / |                                   / |
        //          /   |                                 /   |
        //        /     |                               /     |
        //      /       |                             /       |
        //              |                                     |
        //              |                                    \|/ Z(h)
        //                                                    '
        // printf("%d %d\n", row, col);
        if (!float_equal((float)row, pos.second)) {
          printf("print the %c side of block r/c = (%d, %d)", row > pos.second ? 'l' : 'r', row, col);
          for (float z = (float)col - .5f - pos.first, x = row > pos.second ? row - .5f - pos.second : row + .5f - pos.second, normal[] = {0, 0, 1}; z <= (float)col + .5f - pos.first; z += render_spacing) {  // TODO change to dinamic spacing
            for (float y = -wall_height_2; y <= wall_height_2; y += render_spacing) {
              float zb = sqrt(pow(x, 2) + pow(z, 2));
              int win_x = round_upper((float)win_col / 2 + x * scaling_factor / z + 0.5),        //
                  win_y = round_upper((float)win_row / 2 + (y * scaling_factor / z) / 2 + 0.5);  //

              // printf("zb = %f, win_x = %d, win_y = %d\n", zb, win_x, win_y);

              if ((win_y >= 0 && win_y < win_row) &&  //
                  (win_x >= 0 && win_x < win_col) &&  //
                  zb > z_buffer[win_y][win_x]         //
              )
                z_buffer[win_y][win_x] = zb, display[win_y][win_x] = grayscale[luminance_index(normal, x, y, z)];  //(int)luminance(normal, x, y, z)];
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

  for (int i = 0; i < win_row; i++) {
    for (int j = 0; j < win_col; j++) printf("%c ", display[i][j]);
    printf("\n");
  }
  for (int i = 0; i < win_row; i++) {
    free(display[i]);
    free(z_buffer[i]);
  }
  free(display);
  free(z_buffer);
}

int main() {
#ifdef __linux__
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  win_row = w.ws_row, win_col = w.ws_col;
#elif _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  win_row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1, win_col = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#endif
  printf("win_row = %d, win_col = %d\n", win_row, win_col);
  delay(1.f);

  int ii;
  //   float max_dir = 4 * PI;
  Map *map = new_Map(11, 17);
  for (int i = 0; i < map->row; i++) memset(map->data[i], '9', (unsigned)map->col);
  map->data[6][8] = '@';

  float dir;
#ifdef DEBUG
  for (int i = 0; i < 10; i++) printf("\n");
  for (dir = 0.f; dir <= 12.f / 3.f; dir += rotate_spacing) {
    printf("\n\n\nCalling scan() with angle = %f * PI/2 => %f\n\n", dir, dir * PI_2);
    scan(*map, make_FloatPair(8.f, 5.f), fmodf(dir, 4.f));
    delay(1.f);
    // printf("\e[1;1H\e[2J");
  }
#else
  scan(*map, make_FloatPair(8.f, 5.f), 0);
#endif
  //   new_tan(0, -5.f / 2.f * PI);
  //   printf("%g", 1.f / tanf(PI / 2));
  Map_clear(map);
  // for (float angle = 0; angle < 2 * PI + 0.1; angle += PI / 16) {
  //   printf("%-8g\t(%g\tpi):\t%d\n", angle, angle / PI, sign_of_tan(angle));
  // }
}