// #define DEBUG

#include "../draw.c"
#include "../header.h"
#include "../types.h"

int main() {
  printf(CLEAR);
  Animation *circle = new_Animation("../assets/circle.txt");
  Animation *game_over = new_Animation("../assets/game_over.txt");
  if (circle == NULL) return -1;     // test on null pointer is not sufficient to indicate that the animation is loaded correctly
  if (game_over == NULL) return -1;  // test on null pointer is not sufficient to indicate that the animation is loaded correctly
  printf("circle frames: %d, row %d, col %d\n", circle->frames, circle->row, circle->col);
  printf("game_over frames: %d, row %d, col %d\n", game_over->frames, game_over->row, game_over->col);
  printf("loc of circle data %p\n", circle->data);
  printf("loc of game_over data %p\n", game_over->data);
  // Animation_clear(circle);
  drawAnimation(game_over, 0, 6, 1);
  // drawAnimation(circle, 1, 16, 150);
  Animation_clear(game_over);
  Animation_clear(circle);

  Animation *bad = new_Animation("../assets/bad.txt");
  // if (bad == NULL) return -1;
  Animation_clear(bad);
}