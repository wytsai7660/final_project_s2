// #define DEBUG

#include "../header.h"
#include "../types.h"
#include "../draw.c"

int main() {
    printf(CLEAR);
    Animation *circle = new_animation("../assets/circle.txt");
    Animation *game_over = new_animation("../assets/game_over.txt");
    if (circle == NULL) return -1;
    if (game_over == NULL) return -1;
    printf("circle frames: %d, row %d, col %d\n", circle->frames, circle->row, circle->col);
    printf("game_over frames: %d, row %d, col %d\n", game_over->frames, game_over->row, game_over->col);
    printf("loc of circle data %p\n", circle->data);
    printf("loc of game_over data %p\n", game_over->data);
    // animation_clear(circle);
    drawAnimation(game_over, 0, 6, 1);
    // drawAnimation(circle, 1, 16, 150);
    animation_clear(game_over);
    animation_clear(circle);
}