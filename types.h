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
  int life, hp, hpMax;
  int atk, def, crit, dir;
  int watchTowerCnt;
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
  p->hpMax = 10;
  p->atk = 3;
  p->def = 3;
  p->crit = 10;
  p->watchTowerCnt = 0;
#ifdef DEBUG
  p->life = 99;
  p->hp = 15;
  p->hpMax = 15;
  p->atk = 999;
  p->def = 999;
  p->crit = 99;
  p->watchTowerCnt = 0;
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
    float **data;
    int n_states; // TODO not yet being used, will remove it some time
    int bossState;
    int atk;
    int def;
    int hp;
    int hpMax;
    int crit;
    float *moveDistrube;
    int PlayerMove;
    int EnemyMove;
}Enemy;

Enemy *new_Enemy(PlayerData *p, int n_states) {
    if(n_states < 2) return NULL;
    Enemy *e = malloc(sizeof(Enemy));

    float sum = 0;
    // prob of paper, sccisor, stone
    float *moveDistrube = malloc(sizeof(float) * 3);
    for(int i = 0;i < 3;i++){
        moveDistrube[i] = (float)rand() / RAND_MAX;
        sum += moveDistrube[i];
    }

    // normalize the numbers to sum up to 1
    for(int i = 0;i < 3;i++){
        moveDistrube[i] /= sum;
    }

    float **tmp = malloc(n_states * sizeof(float *));
    for (int i = 0; i < n_states; i++) tmp[i] = memset(malloc(n_states * sizeof(float)), 0, n_states);
    int hp = p->hpMax - 5 + rand_between(0, 10);
    *e = (Enemy){tmp, n_states, 0, rand_between(p->hp/10 + 1, p->hp/3 + 1), rand_between(p->atk/10 + 1, p->atk/2 + 1), hp, hp, rand_between(0, 10), moveDistrube};
    return e;
}

void Enemy_clear(Enemy *e) {
  for (int i = 0; i < e->n_states; i++) free(e->data[i]);
  free(e->data);
  free(e);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


typedef struct
{
    int types;
    int frames;
    int row;
    int col;
    char ****data;
}Animation;

Animation *new_animation(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        return NULL;
    }

    Animation *ani = malloc(sizeof(Animation));
    fscanf(file, "%d", &(ani->types));
    fscanf(file, "%d", &(ani->frames));
    fscanf(file, "%d", &(ani->row));
    fscanf(file, "%d", &(ani->col));

    // allocate memory & read from file
    ani->data = malloc(ani->types * sizeof(char***));
    for (int i = 0; i < ani->types; i++) {
        fgetc(file);
        ani->data[i] = malloc(ani->frames * sizeof(char**));
        for (int j = 0; j < ani->frames; j++) {
            ani->data[i][j] = malloc(ani->row * sizeof(char*));
            for (int k = 0; k < ani->row; k++) {
                ani->data[i][j][k] = malloc((ani->col + 1) * sizeof(char)); // +1 for null terminator
                fgets(ani->data[i][j][k], ani->col + 1, file);
                ani->data[i][j][k][strcspn(ani->data[i][j][k], "\n")] = '\0'; // remove trailing newline
            }
        }
    }
    
    fclose(file);
    return ani;

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
  bool inputLocked;
  // int *enemyMoves;
  // int *playerMoves;
  int enemyOldMoves;
  int playerOldMoves;
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

  // g->enemyMoves = malloc(sizeof(int) * 50);
  // g->playerMoves = malloc(sizeof(int) * 50);
  g->items_enabled = malloc(sizeof(bool) * (sizeof(items_ratio) / sizeof(float)));
  for (unsigned i = 0; i < sizeof(items_ratio) / sizeof(float); i++) g->items_enabled[i] = false;
  return g;
}

void Game_clear(Game *g) {
  free(g->items_enabled);
  free(g);
}

#endif