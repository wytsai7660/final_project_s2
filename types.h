#ifndef TYPES_H_
#define TYPES_H_
#include "header.h"

// TODO need to add a queue<pair<int,int>>

// pair<float,float>
// used for recording dynamically changing coordinate
typedef struct {
  float first, second;
} FloatPair;

FloatPair make_FloatPair(float f1, float f2) { return (FloatPair){f1, f2}; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// pair<int,int>
// used for saving coordinates
typedef struct {
  int first, second;
} IntPair;  // FIXME not using, should be removed or used

IntPair make_IntPair(int i1, int i2) { return (IntPair){i1, i2}; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct __INT_PAIR_NODE__ {
  IntPair data;
  struct __INT_PAIR_NODE__ *next;
} IntPairNode;  // FIXME not using, should be removed or used

// list<pair<int,int>>
// used for saving coordinates
/// TODO remove it and rewrite it in a better way
typedef struct {
  IntPairNode *head, *rear;
  int size;
} IntPairList;  // FIXME not using, should be removed or used

IntPairList *new_IntPairList() {
  IntPairList *l = malloc(sizeof(IntPairList));
  *l = (IntPairList){NULL, NULL, 0};
  return l;
}

void IntPairList_push(IntPairList *l, IntPair p) {
  if (!l->size) l->head = malloc(sizeof(IntPairNode)), l->rear = l->head;
  else l->rear->next = malloc(sizeof(IntPairNode)), l->rear = l->rear->next;
  *(l->rear) = (IntPairNode){p, NULL}, l->size++;
}

void IntPairList_clear(IntPairList *l) {
  IntPairNode *ptr = l->head, *tmp;
  while (ptr != NULL) {
    tmp = ptr;
    ptr = ptr->next;
    free(tmp);
  }
  free(l);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// triple<int,int,int>
typedef struct {
  int first, second, third;
} IntTriple;

IntTriple make_IntTriple(int i1, int i2, int i3) { return (IntTriple){i1, i2, i3}; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct __INT_TRIPLE_NODE__ {
  IntTriple data;
  struct __INT_TRIPLE_NODE__ *next;
} IntTripleNode;

// stack<triple<int, int, int>>
typedef struct {
  IntTripleNode *top;
  int size;
} IntTripleStack;

IntTripleStack *new_IntTripleStack() {
  IntTripleStack *s = malloc(sizeof(IntTripleStack));
  *s = (IntTripleStack){NULL, 0};
  return s;
}

IntTriple IntTripleStack_top(IntTripleStack *s) { return s->size > 0 ? s->top->data : make_IntTriple(-1, -1, -1); }

void IntTripleStack_push(IntTripleStack *s, IntTriple t) {
  IntTripleNode *tmp = malloc(sizeof(IntTripleNode));
  *tmp = (IntTripleNode){t, s->top};
  s->top = tmp, s->size++;
}

void IntTripleStack_pop(IntTripleStack *s) {
  if (s->size <= 0) return;
  IntTripleNode *tmp = s->top;
  s->top = s->top->next;
  free(tmp);
  s->size--;
}

void IntTripleStack_clear(IntTripleStack *s) {
  IntTripleNode *ptr = s->top, *tmp;
  while (ptr != NULL) {
    tmp = ptr;
    ptr = ptr->next;
    free(tmp);
  }
  free(s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
  char *data;
  int front, size;
} CharArray;

CharArray *new_CharArray(int n) {
  if (n <= 0) return NULL;
  CharArray *a = malloc(sizeof(CharArray));
  *a = (CharArray){malloc((unsigned)n * sizeof(char)), 0, 0};
  return a;
}

void CharArray_push_back(CharArray *a, char c) { a->data[a->front + a->size++] = c; }

char CharArray_pop_front(CharArray *a) {
  a->size--;
  return a->data[a->front++];
}

void CharArray_clear(CharArray *a) {
  free(a->data);
  free(a);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
  char **data;
  // @  wall
  //' ' path
  // 0  +HP
  // 1  -HP
  // 2  +ATK
  // 3  -ATK
  // 4  +DEF
  // 5  -DEF
  // 6  enemy
  // 7  watchtower
  // 8  gain item
  // 9  -
  // P  player

  int row, col, path, walked;
} Map;

Map *new_Map(int r, int c) {
  if (r <= 0 || c <= 0) return NULL;
  Map *m = malloc(sizeof(Map));
  char **tmp = malloc((unsigned)r * sizeof(char *));
  for (int i = 0; i < r; i++) tmp[i] = memset(malloc((unsigned)c * sizeof(char)), '@', (unsigned)c);
  *m = (Map){tmp, r, c, (r * c - r - c - 1) / 2, 0};
  return m;
}

void Map_clear(Map *m) {
  for (int i = 0; i < m->row; i++) free(m->data[i]);
  free(m->data);
  free(m);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
  int life, hp;
  int atk, def, crit, dir;
  int watchTowerCnt;
  IntPair pos;
  int *backpack;
} PlayerData;

// item
// teleport: randomly teleport to another place
// blood++: use in battle, heal you life by 5
// defense: use in battle, 90% chance ignore next monster's attack
//

PlayerData *new_PlayerData() {
  PlayerData *p = malloc(sizeof(PlayerData));
  // TODO decide the number
  // *p = (PlayerData){5, 10, 10, 10, 20, 0, make_FloatPair(a, b), array init};
  p->life = 5;
  p->hp = 10;
  p->atk = 3;
  p->def = 3;
  p->crit = rand_between(0, 20);
  p->watchTowerCnt = 0;
  p->backpack = malloc(sizeof(int) * (sizeof(items_ratio)/sizeof(float))); // 3 types of item
  for(int i=0;i < sizeof(items_ratio)/sizeof(float); i++) {
    p->backpack[i] = 0;
  }
  p->dir = 0;
  return p;
}

void PlayerData_clear(PlayerData *p) {
  free(p->backpack);
  free(p);
}

typedef struct {
  char **message;

} MessageNode;


typedef struct {
  int watchTowerCnt;
  int status;
  IntPairList *playerPath;

} Game;

Game *new_Game() {
  Game *g = malloc(sizeof(Game));
  // TODO decide the number
  // *p = (PlayerData){5, 10, 10, 10, 20, 0, make_FloatPair(a, b), array init};
  g->status = 0;
  // 0: menu
  // 1: instruction?
  // 2: map
  // 3: fight
  // 9: game over?
  g->watchTowerCnt = 0;
  g->playerPath = new_IntPairList();
  return g;
}

void Game_clear(Game *g) {
  IntPairList_clear(g->playerPath);
  free(g);
}

#endif