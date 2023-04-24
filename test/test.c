#include "../header.h"
#include "../types.h"

int conv_move(char c) {  // convert movements (w, a, s, d) to direction numbers
  if (toupper(c) == 'W' || toupper(c) == 'A' || toupper(c) == 'S' || toupper(c) == 'D') return abs(7 - (int)toupper(c) % 10 - ((int)toupper(c) % 10 == 7 ? 0 : 1));
  return -1;  // invalid input
}

int main() {
  //   printf("%d %d %d %d\n", conv_move('W'), conv_move('A'), conv_move('S'), conv_move('D'));

  //   int dir;
  //   IntTriple pos = make_IntTriple(1, 1, dir = rand_between(0, 3));  // center (x, y, from_dir)
  //   printf("%d\n", dir);
  //   printf("%d %d %d\n", pos.first, pos.second, pos.third);
  //   for (int i = 0; i < 4; i++) {
  //     printf("from: %d\n", i);
  //     for (int j = 0; j < 3; j++) printf("%d ", (i + 3 + j) % 4);
  //     printf("\n");
  //   }
  //   for (int i = 0; i < 10; i++) printf("%g ", events_ratio[i]);
  char a = 'A', b = 'B';
  char *ptr1 = &a, *ptr2 = &b;
  swap(ptr1, ptr2, sizeof(char));
  printf("%c %c\n", a, b);
}