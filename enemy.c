#include "header.h"
#include "types.h"

char *moves[] = {"Paper", "Scissor", "Stone"};
char *gameResults[] = {"tie", "enemy win", "you win"};

// boss states
// 0: normal mode
// 1: aggressive mode
// 2: defensive mode
float bossTransferMatrix[2][3][3] = {{
                                         // hp below 20
                                         {0.3, 0.1, 0.6},  // normal mode
                                         {0.5, 0.3, 0.2},  // aggressive mode
                                         {0.1, 0.0, 0.9},  // defensive mode
                                     },
                                     {
                                         // damage over 20
                                         {0.9, 0.1, 0.0},
                                         {0.3, 0.3, 0.4},
                                         {0.1, 0.2, 0.7},
                                     }};

int roundRemain = 0;
int policy = 0;

int choose_enemy_move(Enemy e, int last_player_move, int last_enemy_move) {
  int enemy_move = rand_between(0, 2);
  if (!roundRemain) {
    roundRemain = rand_between(2, 5);  // how long will the policy last
    policy = rand_between(0, 4);       // randomly choose a policy
  } else {
    roundRemain--;
  }
  if (policy == 0) enemy_move = last_player_move;                 // choose the move that the player choosed last round
  else if (policy == 1) enemy_move = (last_player_move + 1) % 3;  // choose the move that can win the last round
  else if (policy == 2) enemy_move = (last_player_move + 2) % 3;  // choose the move that can lose the last round
  else if (policy == 3) enemy_move = last_enemy_move;             // choose the same move as last round
  else if (policy == 4) enemy_move = sample(e.move_ratio, 3);     // randomly sample a move according to the move_ratio

  return enemy_move;
}

// real damage = (int) atk * (1 + isCrit ? 2 : 1) / (1 + def / 10)
int solveDamage(PlayerData *p, Enemy *e, Game *g, int playerMove, int enemyMove) {
  int result = (enemyMove - playerMove + 3) % 3;
  if (result == 0) {
    // tie
  } else if (result == 1) {
    // enemy win
    g->isCrit = rand_between(1, 100) <= e->crit;
    g->damage = e->atk * (g->isCrit ? 2 : 1) / (1 + p->def / 10.0);
    if (p->sheild_enabled && rand_between(0, 99) <= 90) g->damage = 0;
    p->sheild_enabled = false;
    p->hp -= (int)g->damage;
  } else if (result == 2) {
    // player win
    g->isCrit = rand_between(1, 100) <= p->crit;
    g->damage = p->atk * (g->isCrit ? 2 : 1) / (1 + e->def / 10.0);
    e->hp -= (int)g->damage;
  }
  return result;
}

// event
// boss hp below 20
// boss got damage over 20
// boss hp is 1
// win in the fight
// equal
// loss
void bossPolicy(Game *g, Enemy *e) {
  if (e->hp <= 20) {
    e->boss_state = sample(bossTransferMatrix[0][e->boss_state], 3);
  } else if (g->damage > 20) {
    e->boss_state = sample(bossTransferMatrix[1][e->boss_state], 3);
  }

  if (e->boss_state == 0) {
    e->atk = 20;
  } else if (e->boss_state == 1) {
    e->atk = 30;
  } else {
    e->def = 20;
  }
}
