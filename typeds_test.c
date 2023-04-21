#include "types.h"

int main() {
  win_row = 10, win_col = 20;
  FloatPair fp = make_FloatPair(0.5f, 0.1f);
  IntPair ip = make_IntPair(24, 25);
  IntTriple it = make_IntTriple(99, 88, 77);
  printf("%f %f\n%d %d\n%d %d %d\n", fp.first, fp.second, ip.first, ip.second, it.first, it.second, it.third);

  /////////////////////////////
  // IntPairNode *ipn;
  // Please test this class for me and report any bugs you discover. Any advice is welcome.
  // IntTripleNode *itn;
  // Please test this class for me and report any bugs you discover. Any advice is welcome.
  /////////////////////////////

  IntPairList *ipl = new_IntPairList();
  // Please test this class for me and report any bugs you discover. Any advice is welcome.
  IntTripleStack *its = new_IntTripleStack();
  // Please test this class for me and report any bugs you discover. Any advice is welcome.

  Map *mp = new_Map();
  for (int i = 0; i < win_row; i++) {
    for (int j = 0; j < win_col; j++) {
      printf("%c ", mp->data[i][j]);
    }
    printf("\n");
  }
  Map_clear(mp);
  free(ipl);
  free(its);
}