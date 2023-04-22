#include "../types.h"

int main() {
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

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  IntPairList *ipl = new_IntPairList();
  // Please test this class for me and report any bugs you discover. Any advice is welcome.
  // tested: wytsai
  for (int i = 0; i < 10; i++) IntPairList_push(ipl, make_IntPair(i, i + 1));
  IntPairNode *ptr = ipl->head;
  while (ptr != NULL) {
    printf("(%d, %d)\n", ptr->data.first, ptr->data.second);
    ptr = ptr->next;
  }
  IntPairList_clear(ipl);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  IntTripleStack *its = new_IntTripleStack();
  // Please test this class for me and report any bugs you discover. Any advice is welcome.
  // tested: wytsai
  for (int i = 0; i < 10; i++) IntTripleStack_push(its, make_IntTriple(i, i + 1, i + 2));
  // while (its->top != NULL) {
  //   printf("(%d, %d, %d)\n", its->top->data.first, its->top->data.second, its->top->data.third);
  //   IntTripleStack_pop(its);
  // }
  IntTripleStack_clear(its);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  Map *mp = new_Map(10, 30);
  for (unsigned i = 0; i < mp->row; i++) {
    for (unsigned j = 0; j < mp->col; j++) {
      printf("%c ", mp->data[i][j]);
    }
    printf("\n");
  }
  Map_clear(mp);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}