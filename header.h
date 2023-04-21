#ifndef HEADER_H_
#define HEADER_H_

#include <ctype.h>
#include <fcntl.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define msg_sleep 1 * 1000 * 1000  // 1sec

int my_rand(int l, int r) {  // get a random number between [l,r] 
  return rand() % (r - l + 1) + l;
}

int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }

//     w         1            (0,-1)
//  a     d   2     0   (-1,0)      (1, 0)
//     s         3            (0, 1)
int dir[4][2] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
unsigned win_row, win_col;

#endif