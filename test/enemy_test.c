#include "../enemy.c"

#include "../header.h"
#include "../types.h"

int main() {
  srand(time(NULL));
  PlayerData *p = new_PlayerData();
  Enemy *e = new_Enemy(p, 5);
  Game *game = new_Game();
  printf("player hp: %d, atk: %d, def: %d\n", p->hp, p->atk, p->def);
  printf("enemy hp: %d, atk: %d, def: %d\n", e->hp, e->atk, e->def);
  int playerMove = 2, enemyMove, result;
  game->playerOldMoves = rand_between(0, 2);
  game->enemyOldMoves = rand_between(0, 2);
  enemyMove = enemyNextMove(game->playerOldMoves, game->enemyOldMoves);
  result = solveDamage(p, e, game, playerMove, enemyMove);

  printf("damage: %f, result: %d\n", game->damage, result);
  printf("player hp: %d, atk: %d, def: %d\n", p->hp, p->atk, p->def);
  printf("enemy hp: %d, atk: %d, def: %d\n", e->hp, e->atk, e->def);

  printf("boss state: %d\n", e->boss_state);
  game->damage = 100;
  bossPolicy(game, e);
  printf("boss state: %d\n", e->boss_state);
}