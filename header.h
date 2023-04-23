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

int rand_between(int min, int max) {  // get a random number between [min, max] 
  return rand() % (max - min + 1) + min;
}

int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }

//     w         1            (0,-1)
//  a     d   2     0   (-1,0)      (1, 0)
//     s         3            (0, 1)
int direction[4][2] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
unsigned win_row, win_col;

float events_ratio[] = {0.075f, 0.025f, 0.075f, 0.025f, 0.075f, 0.025f, 0.2f, 0.05f, 0.05f, 0.4f};
// 0  回血  7.5%
// 1  扣血  2.5%
// 2  加功  7.5%
// 3  減功  2.5%
// 4  加防  7.5%
// 5  減防  2.5%
// 6  遭遇敵人  20%
// 7  可透視附近敵人  5%
// 8  獲得道具  5%
// 9  無事發生  40%

#endif