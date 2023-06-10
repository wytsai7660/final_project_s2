#include "../header.h"
#include "../types.h"
#include "../draw.c"

int main() {
    printf(CLEAR);
    Animation *circle = new_animation("../assets/circle.txt");
    if (circle == NULL) return -1;
    printf("types: %d, frames: %d, row %d, col %d\n", circle->types, circle->frames, circle->row, circle->col);
    drawAnimation(circle, 1, 0, 2, 1);
}