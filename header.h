#ifndef HEADER_H_
#define HEADER_H_

#define _POSIX_C_SOURCE 199309L

#ifdef __linux__
  #include <fcntl.h>
  #include <sys/ioctl.h>
  #include <termios.h>
  #include <unistd.h>
#elif _WIN32
  #define NOMINMAX
  #include <conio.h>
  #include <windows.h>
#endif

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RED "\e[31m"
#define GRN "\e[32m"
#define YEL "\e[33m"
#define BLU "\e[34m"
#define MAG "\e[35m"
#define CYN "\e[36m"
#define WHT "\e[37m"
#define GRY "\e[90m"
#define ALT "\e[7;17m"  // highlight the current option
#define RESET "\e[0m"
#define CLEAR "\e[1;1H\e[2J"
#define HIDE_CURSOR "\e[?25l"
#define SHOW_CURSOR "\e[?25h"
#define MOVE_TO(r, c) "\e["(r) ";"(c) "H"  // TODO test it and switch to it

#define PI 3.14159265358979323846f
#define PI_2 1.57079632679489661923f

#define MINIMAP_SIZE 7
#define TEXT_AREA_HEIGHT 17  // smallmapsize * 2 + 3
#define MAP_AREA_WIDTH 33    // text_area_height * 2 - 1

#define MAP_ROW 35
#define MAP_COL 87

#define EPSILON 1e-6f  // fix some issues caused by float point precision

#define ITEM_TYPES 4

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// sleep t seconds
void delay(float t) {
  if (t <= 0) return;
  nanosleep(&(struct timespec){(time_t)t, (long)(t * 1000000000) % 1000000000}, NULL);
}

int rand_between(int l, int r) { return rand() % (r - l + 1) + l; }  // get a random number between [l, r] 

int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }

bool float_equal(int i, float f) { return fabsf((float)i - f) < EPSILON; }

// a swap function that can swap almost anything (in a beautiful way)
void swap(void *a, void *b, size_t size) {
  if (a == b) return;
  char *ptr1 = a, *ptr2 = b;
  while (size--) {
    (*ptr1) ^= *ptr2, (*ptr2) ^= *ptr1, (*ptr1) ^= *ptr2;
    ptr1++, ptr2++;
  }
}

// return a number (index) based on the probability distribution array
int sample(float ratio[], int length) {
  float randfloat = (float)rand() / (float)RAND_MAX;  // [0, 1]
  float sum = 0;
  for (int i = 0; i < length; i++) {
    sum += ratio[i];
    if (sum > randfloat) return i;
  }
  return -1;
}

// A/D rotate, W move
/*                   ,
                    /^\   W(2)
                   //|\\
                  />|||<\
                    [|]
                    [|]
                    """
          A(1)  __-- P --__  D(-1)
              |/_         _\|             */

int convert_move(char c) {
  if (toupper(c) == 'W') return 2;   // move forward
  if (toupper(c) == 'A') return 1;   // turn left
  if (toupper(c) == 'D') return -1;  // turn right
  return 0;                          // invalid input
}

void clearInputBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int win_row, win_col;
//     w         1            (0,-1)
//  a     d   2     0   (-1,0)      (1, 0)
//     s         3            (0, 1)
const int direction[4][2] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};

// constants for 3d rendering
const float fov = PI * 2 / 3;  // 120 degree
const float scaling_factor = 50;
const float render_spacing = 0.001f;
const char grayscale[] = ".,-~:;=!*#$@";  // 12 chars
// ".'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";  // 69 chars
// " (int)(-69.f * (normal[0] * x + normal[1] * y + normal[2] * z) * powf(powf(x, 2) + powf(y, 2) + powf(z, 2), -0.5))"
const float wall_height_2 = .5f;

const float rotate_spacing = 1.f / 12.f;
const float move_spacing = .1f;

const float events_ratio[] = {0.075f, 0.025f, 0.075f, 0.025f, 0.075f, 0.025f, 0.1f, 0.05f, 0.15f, 0.4f};
// 0  +HP
// 1  -HP
// 2  +ATK
// 3  -ATK
// 4  +DEF
// 5  -DEF
// 6  enemy
// 7  watchtower
// 8  gain item
// 9  -

const char *const items_name[] = {"Blinkstone", "Vitality Potion", "Guardian Shield", "Magic Compass"};
const float items_ratio[] = {0.1f, 0.4f, 0.2f, 0.3f};
const bool items_maze_usability[] = {true, false, false, true};

// item
// 0 Blinkstone: randomly teleport to another place
// 1 Vitality Potion: use in battle, heal you life by 5
// 2 Guardian Shield: use in battle, 90% chance ignore next monster's attack
// 3 Magic Compass: Activate to reveal the location of hidden treasures or secret paths in the game world.

clock_t start, end;
float cpu_time_used;
int tick;  // TODO move into main() in game.c

void one_tick(clock_t start, clock_t end) {
  printf("\e[%d;%dH" HIDE_CURSOR, win_row - 1, 1);
  tick = tick < INT_MAX ? tick + 1 : 0;
  cpu_time_used = ((float)(end - start)) / CLOCKS_PER_SEC;
  printf("Time taken: %f seconds, tick: %d\n", cpu_time_used, tick);
  delay(.03f - cpu_time_used * 2);
}

// #define DEBUG

#endif