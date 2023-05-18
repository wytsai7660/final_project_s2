#include "../header.h"

int main() {
  srand((unsigned)time(NULL));
  int i1 = 10, i2 = 24;
  char c1 = 'A', c2 = 'C';
  float f1 = 0.5f, f2 = -45.3f;
  printf("%d\n", rand_between(0, 3));
  printf("%d %d %c %c %g %g\n", i1, i2, c1, c2, f1, f2);
  for (int i = 0; i < 1e6; i++) {  // just to show that my swap() function works fine and it's super fast
    swap(&i1, &i2, sizeof(int));
    swap(&c1, &c2, sizeof(char));
    swap(&f1, &f2, sizeof(float));
  }
  printf("%d %d %c %c %g %g\n", i1, i2, c1, c2, f1, f2);
  int i3 = 10, i4 = 20;
  unsigned i5 = 10, i6 = 20;
  printf("%d\n%d", rand_between(i3, i4), rand_between(i5, i6));
}