#include "draw.c"
#include "enemy.c"
#include "header.h"
#include "map.c"
#include "types.h"


void clearPanel() {
  for (int i = win_row - TEXT_AREA_HEIGHT; i < win_row; i++) {
    printf("\e[K\e[B");
  }
}

void battleLoop(Game *game, PlayerData *player, Map *map) {
  Enemy *enemy = new_Enemy(player, game->is_boss);
  game->playerOldMoves = rand_between(0, 2);
  game->enemyOldMoves = rand_between(0, 2);
  char ch;
  int choice = 0, chosen = -1, result;
  player->hp = player->max_hp;

  while (game->status == 3) {
    start = clock();

    bool updateAnimationOnly = false;
    ssize_t bytesRead = read(STDIN_FILENO, &ch, 1);
    clearInputBuffer();

    if (bytesRead == 1 && !game->input_locked) {
      if (toupper(ch) == 'A') {
        printf(CLEAR);
        choice = (choice - 1 + 3) % 3;
      } else if (toupper(ch) == 'D') {
        printf(CLEAR);
        choice = (choice + 1) % 3;
      } else if (ch == '\n') {
        printf(CLEAR);
        chosen = choice;
      } else if (toupper(ch) == 'E') {
        chooseItem(player, game);
        printf(CLEAR);
      } else {
        updateAnimationOnly = true;
      }
    } else {
      updateAnimationOnly = true;
    }

    drawSolidBox(13, 25, tick / 30 % 3, 9, win_col / 4);
    printf("\e[%d;%dH", 9 + 14, win_col / 4 - 12);
    drawHp(player->hp, player->max_hp, player->sheild_enabled);
    drawSolidBox(13, 25, tick / 30 % 3, 5, win_col / 4 * 3);
    printf("\e[%d;%dH", 5 + 14, win_col / 4 * 3 - 12);
    drawHp(enemy->hp, enemy->max_hp, false);

    if (updateAnimationOnly) {
      end = clock();
      one_tick(start, end);
      continue;
    }

    if(game->items_enabled[1]) {
      player->hp = min(player->max_hp, player->hp + 5);
      player->backpack[1]--;
      game->items_enabled[1] = false;
    }
    if(game->items_enabled[2]) {
      player->sheild_enabled = true;
      player->backpack[2]--;
      game->items_enabled[2] = false;
    }


    drawBox(TEXT_AREA_HEIGHT, win_col - MAP_AREA_WIDTH, win_row - TEXT_AREA_HEIGHT - 1, 1);
    drawBox(TEXT_AREA_HEIGHT, MAP_AREA_WIDTH, win_row - TEXT_AREA_HEIGHT - 1, win_col - MAP_AREA_WIDTH + 1);
    drawMiniMap(map, &player->pos, MINIMAP_SIZE, 0, win_row - TEXT_AREA_HEIGHT, win_col - MAP_AREA_WIDTH + 3);
    printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT, 3);
    printf("[A][D] To Select   [ENTER] To Choose   [E] To Open Backpack");
    drawChoice(moves, choice, win_row - TEXT_AREA_HEIGHT + 2, 3);

    if (chosen != -1) {
      printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT + 4, 3);
      int enemyMove = enemyNextMove(game->playerOldMoves, game->enemyOldMoves);
      result = solveDamage(player, enemy, game, chosen, enemyMove);
      printf("damage: %.2f\n", game->damage);
      printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT + 5, 3);
      printf("You threw %s and enemy threw %s, %s!", moves[chosen], moves[enemyMove], gameResults[result]);

      if (enemy->hp <= 0) {
        printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT + 6, 3);
        printf("you beat the enemy!");
        game->status = 2;
      }
      if (player->hp <= 0) {
        printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT + 6, 3);
        printf("you loose!");
        player->life--;
        game->status = 2;
      }

      chosen = -1;
    }

    drawStatusBar(player, false, win_row - 3, 3);

    end = clock();
    one_tick(start, end);
  }

  if(player->life <= 0) {
    game->status == 9;
  }
  if(game->is_boss && player->hp <= 0) {
    game->status = 9;
  }
  if(game->is_boss && enemy->hp <= 0) {
    game->status = 8;
  }
}
