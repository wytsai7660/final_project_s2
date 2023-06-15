#ifndef ANIMATION_C_
#define ANIMATION_C_

#include "draw.c"
#include "header.h"
#include "types.h"

void animateHit(Animation *ani, int tick, bool *input_locked, int y, int x) {

  tick = tick % 9;
  if(tick == 8) {
    *input_locked = false;
  }
  if(tick == 0|| tick == 7) {
    drawAnimation(ani, 0, y, x);
  } else if(tick == 2 || tick == 6) {
    printf(GRY);
    drawAnimation(ani, 0, y, x);
  } else if(tick == 3 || tick == 5) {
    printf(RED);
    drawAnimation(ani, 0, y, x);
  } else if(tick == 4){
    printf("\e[31;1m");
    drawAnimation(ani, 0, y, x + 1);
  }
  printf(RESET);
}

#endif