// #include "../3d_renderer.c"
#include "../header.h"
#include "../map.c"
#include "../types.h"

int conv_move(char c) {  // convert movements (w, a, s, d) to direction numbers
  if (toupper(c) == 'W' || toupper(c) == 'A' || toupper(c) == 'S' || toupper(c) == 'D') return abs(7 - (int)toupper(c) % 10 - ((int)toupper(c) % 10 == 7 ? 0 : 1));
  return -1;  // invalid input
}

int main() {
  char a = '6', b = '6', c = '6', d = '6';
  swap(&a, &b, 1);
  swap(&c, &d, 1);
  float f = -1.f / 0.f;
  printf("%f, isinf=%d, isnan=%d, isnormal=%d\n", f, isinf(f), isnan(f), isnormal(f));
  int arr[ITEM_TYPES] = {};
  for (int i = 0; i < ITEM_TYPES; i++) printf("%d ", arr[i]);
}