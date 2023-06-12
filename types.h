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
  for (int i = 0; i < r; i++) tmp[i] = memset(malloc((unsigned)c * sizeof(char)), '@', (unsigned)c * sizeof(char));
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
  int life, hp, max_hp;
  int atk, def, crit, dir;
  int watchTowerCnt;
  bool sheild_enabled;
  IntPair pos;
  int backpack[ITEM_TYPES];
  // item
  // 0 teleport: randomly teleport to another place
  // 1 blood++: use in battle, heal you life by 5
  // 2 defense: use in battle, 90% chance ignore next monster's attack
} PlayerData;

PlayerData *new_PlayerData() {
  PlayerData *p = malloc(sizeof(PlayerData));
  p->life = 5;
  p->hp = 10;
  p->max_hp = 10;
  p->atk = 3;
  p->def = 3;
  p->crit = 10;
  p->watchTowerCnt = 0;
  p->sheild_enabled = false;
#ifdef DEBUG
  p->life = 99;
  p->hp = 15;
  p->max_hp = 15;
  p->atk = 999;
  p->def = 999;
  p->crit = 99;
  p->watchTowerCnt = 0;
  p->sheild_enabled = false;
#endif
  for (unsigned i = 0; i < ITEM_TYPES; i++) p->backpack[i] = 0;  // currently 4 types of item
#ifdef DEBUG
  for (unsigned i = 0; i < ITEM_TYPES; i++) p->backpack[i] = 50;
#endif
  p->dir = 0;
  return p;
}

void PlayerData_clear(PlayerData *p) { free(p); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
  int boss_state;  // TODO decide boss states
  // 0 not boss
  // 1
  // 2
  int atk;
  int def;
  int hp;
  int max_hp;
  int crit;
  float move_ratio[3];
} Enemy;

Enemy *new_Enemy(PlayerData *p, bool boss) {
  Enemy *e = malloc(sizeof(Enemy));
  e->boss_state = boss;
  if (boss) {
    e->atk = rand_between(p->hp / 4 + 1, p->hp / 3 + 1);
    e->def = rand_between(p->atk / 7 + 1, p->atk / 2 + 1);
    e->hp = p->max_hp + 20;
    e->max_hp = e->hp;
    e->crit = rand_between(40, 80);
  } else {
    e->atk = rand_between(p->hp / 10 + 1, p->hp / 3 + 1);
    e->def = rand_between(p->atk / 7 + 1, p->atk / 2 + 1);
    e->hp = p->max_hp + rand_between(-5, 5);
    e->max_hp = e->hp;
    e->crit = rand_between(0, 10);
  }
  
  // prob of paper, sccisor, stone
  // TODO maybe switch to a better way to generate the ratio
  float sum = 0;
  for (int i = 0; i < 3; i++) e->move_ratio[i] = (float)rand() / (float)RAND_MAX, sum += e->move_ratio[i];
  for (int i = 0; i < 3; i++) e->move_ratio[i] /= sum;  // normalize to make the sum = 1

  return e;
}

void Enemy_clear(Enemy *e) { free(e); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
  int frames;
  int row;
  int col;
  char ***data;
} Animation;
/// @note test on null pointer is not sufficient to indicate that the animation is loaded correctly (important)

Animation *new_Animation(const char *filename) {  // FIXME const
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    printf("Failed to open file: %s\n", filename);
    return NULL;
  }

  Animation *ani = malloc(sizeof(Animation));
  if (fscanf(file, "%d", &(ani->frames)) == EOF || fscanf(file, "%d", &(ani->row)) == EOF || fscanf(file, "%d", &(ani->col)) == EOF || fgetc(file) != '\n') {
    printf("%s: file is incomplete!\n", filename);
    free(ani);
    return NULL;
  }

  // allocate memory & read from file
  ani->data = malloc((unsigned)ani->frames * sizeof(char **));
  for (int i = 0; i < ani->frames; i++) {
    ani->data[i] = malloc((unsigned)ani->row * sizeof(char *));
    for (int j = 0; j < ani->row; j++) {
      ani->data[i][j] = malloc((unsigned)(ani->col + 1) * sizeof(char));  // +1 for null terminator
      if (fgets(ani->data[i][j], ani->col + 1, file) == NULL) printf("%s: file is incomplete! can't fix now, plz clear it\n", filename);
      else ani->data[i][j][strcspn(ani->data[i][j], "\n")] = '\0';  // remove trailing newline
#ifdef DEBUG
      printf("frame: %-3d line: %-3d content: %s\n", i, j, ani->data[i][j]);
#endif
    }
  }
  fclose(file);
  return ani;
}

void Animation_clear(Animation *ani) {
  if (ani == NULL) return;
  for (int i = 0; i < ani->frames; i++) {
    for (int j = 0; j < ani->row; j++) {
      free(ani->data[i][j]);
    }
    free(ani->data[i]);
  }
  free(ani->data);
  free(ani);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
  int watchTowerCnt;
  int status;
  int round;
  bool *items_enabled;
  bool isCrit;
  float damage;
  bool input_locked;
  bool is_boss;
  int gained_item;
  char event;
  int last_enemy_move;
  int last_player_move;
  IntPairList *playerPath;
} Game;

Game *new_Game() {
  Game *g = malloc(sizeof(Game));
  g->status = 0;
  g->round = 0;
  // 0: menu
  // 1: instruction?
  // 2: map
  // 3: fight
  // 8: win
  // 9: game over?
  g->event = '9';
  g->input_locked = false;
  g->is_boss = false;
  g->items_enabled = malloc(sizeof(bool) * (sizeof(items_ratio) / sizeof(float)));
  for (unsigned i = 0; i < sizeof(items_ratio) / sizeof(float); i++) g->items_enabled[i] = false;
  return g;
}

void Game_clear(Game *g) {
  free(g->items_enabled);
  free(g);
}

#endif