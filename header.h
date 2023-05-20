#ifndef HEADER_H_
#define HEADER_H_

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef __linux__
  #include <sys/ioctl.h>
  #include <unistd.h>
#elif _WIN32
  #define NOMINMAX
  #include <windows.h>
#endif

#define PI 3.14159265358979323846f
#define PI_2 1.57079632679489661923f

#define msg_sleep 1 * 1000 * 1000  // 1sec

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
const float rotate_spacing = 0.01f;

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

#endif