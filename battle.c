#include "animation.c"
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
  Animation *skeleton = new_Animation("assets/skeleton.txt");
  Animation *knight = new_Animation("assets/knight.txt");
  if (knight == NULL) return;
  if (skeleton == NULL) return;

  game->last_player_move = rand_between(0, 2);
  game->last_enemy_move = rand_between(0, 2);
  char ch;
  int choice = 0, chosen = -1, result, current_tick;
  player->hp = player->max_hp;
  game->input_locked = false;

  printf(CLEAR);
  drawBox(TEXT_AREA_HEIGHT, win_col - MAP_AREA_WIDTH, win_row - TEXT_AREA_HEIGHT, 1);
  drawBox(TEXT_AREA_HEIGHT, MAP_AREA_WIDTH, win_row - TEXT_AREA_HEIGHT, win_col - MAP_AREA_WIDTH + 1);
  drawMiniMap(map, &player->pos, MINIMAP_SIZE, 0, win_row - TEXT_AREA_HEIGHT + 1, win_col - MAP_AREA_WIDTH + 3);
  printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT + 1, 3);
  printf("[A][D] To Select   [ENTER] To Choose   [E] To Open Backpack");
  drawChoice(moves, choice, win_row - TEXT_AREA_HEIGHT + 3, 3);
  drawStatusBar(player, false, win_row - 2, 3);

  while (game->status == 3 || game->input_locked) {
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

    // animation
    if (result == 1 && game->input_locked) {
      animateHit(knight, (tick - current_tick), &(game->input_locked), win_row - TEXT_AREA_HEIGHT - knight->row - 4, (win_col / 4) - (knight->col / 2));
    } else {
      drawAnimation(knight, tick / 7 % knight->frames, win_row - TEXT_AREA_HEIGHT - knight->row - 4, (win_col / 4) - (knight->col / 2));
    }
    printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT - 3, (win_col / 4) - (knight->col / 2));
    drawHp(player->hp, player->max_hp, player->sheild_enabled);

    if (result == 2 && game->input_locked) {
      animateHit(skeleton, (tick - current_tick), &(game->input_locked), win_row - TEXT_AREA_HEIGHT - knight->row - 4, (win_col / 4 * 3) - (skeleton->col / 2));
    } else {
      drawAnimation(skeleton, tick / 7 % skeleton->frames, win_row - TEXT_AREA_HEIGHT - knight->row - 4, (win_col / 4 * 3) - (skeleton->col / 2));
    }
    printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT - 3, (win_col / 4 * 3) - (skeleton->col / 2));
    drawHp(enemy->hp, enemy->max_hp, false);

    if (updateAnimationOnly) {
      end = clock();
      one_tick(start, end);
      continue;
    }

    if (game->items_enabled[1]) {
      player->hp = min(player->max_hp, player->hp + 5);
      player->backpack[1]--;
      game->items_enabled[1] = false;
    }
    if (game->items_enabled[2]) {
      player->sheild_enabled = true;
      player->backpack[2]--;
      game->items_enabled[2] = false;
    }

    drawBox(TEXT_AREA_HEIGHT, win_col - MAP_AREA_WIDTH, win_row - TEXT_AREA_HEIGHT, 1);
    drawBox(TEXT_AREA_HEIGHT, MAP_AREA_WIDTH, win_row - TEXT_AREA_HEIGHT, win_col - MAP_AREA_WIDTH + 1);
    drawMiniMap(map, &player->pos, MINIMAP_SIZE, 0, win_row - TEXT_AREA_HEIGHT + 1, win_col - MAP_AREA_WIDTH + 3);
    printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT + 1, 3);
    printf("[A][D] To Select   [ENTER] To Choose   [E] To Open Backpack");
    drawChoice(moves, choice, win_row - TEXT_AREA_HEIGHT + 3, 3);

    if (chosen != -1) {
      printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT + 5, 3);
      int enemy_move = choose_enemy_move(*enemy, game->last_player_move, game->last_enemy_move);
      result = solveDamage(player, enemy, game, chosen, enemy_move);
      printf("damage: %.2f\n", game->damage);
      printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT + 6, 3);
      printf("You threw %s and enemy threw %s, %s!", moves[chosen], moves[enemy_move], gameResults[result]);

      if (result != 0) {
        game->input_locked = true;
        current_tick = tick + 1;
      }

      if (enemy->hp <= 0) {
        printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT + 7, 3);
        printf("you beat the enemy!");
        enemy->hp = 0;
        if (game->is_boss) {
          game->status = 8;
        } else {
          game->status = 2;
        }
      }
      if (player->hp <= 0) {
        printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT + 7, 3);
        printf("you loose!");
        player->hp = 0;
        player->life--;
        if (game->is_boss) {
          game->status = 9;
        } else {
          game->status = 2;
        }
      }

      chosen = -1;
    }

    drawStatusBar(player, false, win_row - 2, 3);
    // printf("\e[%d;%dH", win_row, 1);
    // printf("enemy atk: %d, enemy def: %d", enemy->atk, enemy->def);

    end = clock();
    one_tick(start, end);
  }

  if (player->life <= 0) {
    game->status = 9;
  }
  
  clearInputBuffer();
  delay(1.5f);
  Animation_clear(skeleton);
  Animation_clear(knight);
  Enemy_clear(enemy);
}
