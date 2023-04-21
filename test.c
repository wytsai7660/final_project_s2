#include "types.h"

int main() {
  FloatPair fp = make_FloatPair(0.5f, 0.1f);
  IntPair ip = make_IntPair(24, 25);
  IntTriple it = make_IntTriple(99, 88, 77);
  printf("%f %f\n%d %d\n%d %d %d\n", fp.first, fp.second, ip.first, ip.second, it.first, it.second, it.third);

  /////////////////////////////
  IntPairNode *ipn;
  // Please test this class for me and report any bugs you discover. Any advice is welcome.
  IntTripleNode *itn;
  // Please test this class for me and report any bugs you discover. Any advice is welcome.
  /////////////////////////////

  IntPairList *ipl = new_IntPairList();
  // Please test this class for me and report any bugs you discover. Any advice is welcome.
  IntTripleStack *its = new_IntTripleStack();
  // Please test this class for me and report any bugs you discover. Any advice is welcome.

  free(ipl);
  free(its);
}