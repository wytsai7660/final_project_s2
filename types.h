#ifndef TYPES_H_
#define TYPES_H_
#include "header.h"

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
} IntPair;

IntPair make_IntPair(int i1, int i2) { return (IntPair){i1, i2}; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct __INT_PAIR_NODE__ {
  IntPair data;
  struct __INT_PAIR_NODE__ *next;
} IntPairNode;

// list<pair<int,int>>
// used for saving coordinates
/// @todo remove it and rewrite it in a better way
typedef struct {
  IntPairNode *head, *rear;
  unsigned size;
} IntPairList;

IntPairList *new_IntPairList() {
  IntPairList *l = malloc(sizeof(IntPairList));
  *l = (IntPairList){NULL, NULL, 0};
  return l;
}

void IntPairList_push(IntPairList *l, IntPair p) {
  if (!l->size)
    l->head = malloc(sizeof(IntPairNode)), l->rear = l->head;
  else
    l->rear->next = malloc(sizeof(IntPairNode)), l->rear = l->rear->next;
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
  unsigned size;
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
  unsigned front, size;
} CharArray;

CharArray *new_CharArray(unsigned n) {
  CharArray *a = malloc(sizeof(CharArray));
  *a = (CharArray){malloc(n * sizeof(char)), 0, 0};
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

  unsigned row, col, path, walked;
} Map;

Map *new_Map(unsigned r, unsigned c) {
  Map *m = malloc(sizeof(Map));
  char **tmp = malloc(r * sizeof(char *));
  for (unsigned i = 0; i < r; i++) tmp[i] = memset(malloc(c * sizeof(char)), '@', c);
  *m = (Map){tmp, r, c, (r * c - r - c - 1) / 2, 0};
  return m;
}

void Map_clear(Map *m) {
  for (unsigned i = 0; i < m->row; i++) free(m->data[i]);
  free(m->data);
  free(m);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
  unsigned life, hp;
  int atk, def, crit, dir;
  FloatPair pos;
  int backpack[10];  // 10 types of item
} PlayerData;

PlayerData *new_PlayerData() {
  PlayerData *p = malloc(sizeof(PlayerData));
  // TODO: decide the number
  // *p = (PlayerData){5, 10, 10, 10, 20, 0, make_FloatPair(a, b), array init};
  p->life = 5;
  p->hp = 5;
  p->atk = 10;
  p->def = 10;
  p->crit = rand_between(0, 20);
  // TODO: init player position
  // p->pos = make_FloatPair(0, 0);
  // p->dir = 0;
  return p;
}

void PlayerData_clear(PlayerData *p) { free(p); }

#endif