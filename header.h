#ifndef HEADER_H_
#define HEADER_H_

#define _POSIX_C_SOURCE 199309L

#ifdef __linux__
  #include <sys/ioctl.h>
  #include <unistd.h>
  #include <termios.h>
#elif _WIN32
  #define NOMINMAX
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
#include <termios.h>
#include <time.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define ATI "\x1B[7;17m"
#define RESET "\x1B[0m"

#define PI 3.14159265358979323846f
#define PI_2 1.57079632679489661923f

#define epsilon 1e-6f  // fix some issues caused by float point precision

// #define msg_sleep 1 * 1000 * 1000  // 1sec

void delay(float t) {  // t sec
  if (t <= 0) return;
  nanosleep(&(struct timespec){(time_t)t, (long)(t * 1000000000) % 1000000000}, NULL);
}

int rand_between(int l, int r) { return rand() % (r - l + 1) + l; }  // get a random number between [l, r] 

int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }

// a swap function that can swap almost anything (in a beautiful way)
void swap(void* a, void* b, size_t size) {
  if (a == b) return;
  char *ptr1 = a, *ptr2 = b;
  while (size--) {
    (*ptr1) ^= *ptr2, (*ptr2) ^= *ptr1, (*ptr1) ^= *ptr2;
    ptr1++, ptr2++;
  }
}

//     w         1            (0,-1)
//  a     d   2     0   (-1,0)      (1, 0)
//     s         3            (0, 1)
const int direction[4][2] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
int win_row, win_col;

const float fov = PI * 2 / 3;  // 120 degree
const float scaling_factor = 30;
const float render_spacing = 0.001f;
const float wall_height = 1;
const float rotate_spacing = 1.f / 6.f;

const float events_ratio[] = {0.075f, 0.025f, 0.075f, 0.025f, 0.075f, 0.025f, 0.2f, 0.05f, 0.05f, 0.4f};
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

const float items_ratio[] = {0.2f, 0.5f, 0.3f};
// item
// teleport: randomly teleport to another place
// blood++: use in battle, heal you life by 5
// defense: use in battle, 90% chance ignore next monster's attack
//

// sample a number from prob distrubution
int sample(float prob[], int plen) {
  float randfloat = (float)rand() / RAND_MAX;
  float sum = 0;
  for (int idx = 0; idx < plen; idx++) {
    sum += prob[idx];
    if (sum > randfloat) {
      return idx;
    }
  }
  return -1;
}

#endif