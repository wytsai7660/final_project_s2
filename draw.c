#ifndef DRAW_C_
#define DRAW_C_

#include "header.h"
#include "types.h"

const char *charArray[] = {"Attack ", "Run ", "Items ", "Hello "};

void drawBox(int height, int width, int y, int x) {
  printf("\e[%d;%dH", y, x);
  printf("\u250C");
  for (int i = 0; i < width - 2; i++) printf("\u2500");
  printf("\u2510\n");
  printf("\e[%d;%dH", ++y, x);

  for (int i = 0; i < height - 2; i++) {
    printf("\u2502");
    for (int j = 0; j < width - 2; j++) printf(" ");
    printf("\u2502\n");
    printf("\e[%d;%dH", ++y, x);
  }

  printf("\u2514");
  for (int i = 0; i < width - 2; i++) printf("\u2500");
  printf("\u2518\n");
}

// coordinate on center
void drawSolidBox(int height, int width, int tick, int y, int x) {
  if (tick == 0) {
    for (int i = 0; i < height; i++) {
      printf("\e[%d;%dH", y + i, x - width / 2);
      for (int j = 0; j < width; j++) {
        printf("%c", j % 2 ? ' ' : '@');
      }
      printf("\n");
    }
  } else if (tick == 1) {
    for (int i = 0; i < height; i++) {
      printf("\e[%d;%dH", y + i, x - width / 2);
      for (int j = 0; j < width; j++) {
        printf("%c", j % 2 ? ' ' : '.');
      }
      printf("\n");
    }
  } else {
    for (int i = 0; i < height; i++) {
      printf("\e[%d;%dH", y + i, x - width / 2);
      for (int j = 0; j < width; j++) {
        printf("%c", j % 2 ? ' ' : '+');
      }
      printf("\n");
    }
  }
}

void drawAnimation(Animation *ani, int frame, int y, int x) {
  printf("\e[%d;%dH", y, x);
  for (int i = 0; i < ani->row; i++) {
    for (int j = 0; j < ani->col; j++) {
      printf("%c", ani->data[frame][i][j]);
    }
    printf("\n");
    printf("\e[%d;%dH", ++y, x);
  }
}

void drawHp(int hp, int hpMax, bool draw_sheild) {
  if (hp < 0 || hpMax <= 0) return;

  printf("HP: |");
  if ((float)hp < hpMax * 0.3) {
    printf("\e[7;31m");
  } else if ((float)hp <= hpMax * 0.5) {
    printf("\e[7;33m");
  } else {
    printf("\e[7;32m");
  }

  for (int i = 0; i < hpMax; i++) {
    if (hp == i) {
      printf(RESET);
    }
    printf(" ");
  }

  if (draw_sheild) {
    printf(RESET "|  ⛨\n");
  } else {
    printf(RESET "|\n");
  }
}
void drawChoice(char *option[], int choice, int y, int x) {
  printf("\e[%d;%dH", y, x);
  for (int i = 0; i < 3; i++) {
    if (i == choice) printf(ALT);
    printf("%s", option[i]);
    printf(RESET "   ");
  }
  printf("\n");
}

void drawStatusBar(PlayerData *p, bool printHp, int y, int x) {
  printf("\e[%d;%dH", y, x);
  printf("LIFE: ");
  printf(RED);

  for (int i = 0; i < 5; i++) {
    if (p->life > i) {
      printf("❤  ");
    } else {
      printf("    ");
    }
  }

  printf(RESET);
  // 26
  // for(int i=0;i<6+4*5)

  printf("       | ATK: %-10d | DEF: %-10d | CRIT: %d%%         | ", p->atk, p->def, p->crit);

  if (printHp) drawHp(p->max_hp, p->max_hp, false);
}

void drawBackpack(PlayerData *p, Game *g, int choice, int y, int x) {
  drawBox(8, 70, y, x);
  printf("\e[%d;%dH", y + 1, x + 2);
  printf("BACKPACK");
  printf("\e[%d;%dH  ", y + 3, x + 2);

  for (int i = 0; i < 4; i++) {
    if (p->backpack[i] == 0 || g->status == 2 && !items_maze_usability[i] || g->status == 3 && items_maze_usability[i]) {
      printf(GRY);
    } else {
      if (g->items_enabled[i]) printf(YEL);
    }
    if (i == choice) printf(ALT);
    printf("%s", items_name[i]);
    printf(RESET);
    printf("   ");
  }

  printf("\e[%d;%dH  ", y + 4, x + 2);
  printf("    %-17d%-18d%-15d%d", p->backpack[0], p->backpack[1], p->backpack[2], p->backpack[3]);

  printf("\e[%d;%dH", y + 2, x + 2);
  printf("\e[%d;%dH", y + 6, x + 2);
  printf("     [A][D] To Choose item  [ENTER] To Activate  [Q] To Leave");
}

void drawMiniMap(Map *map, IntPair *playerPos, int smallMapSize, int watchTowerCnt, int y, int x) {
  // printf("player pos %d, %d\n", playerPos->first, playerPos->second);
  printf("\e[%d;%dH", y, x);

  int xcenter = max(smallMapSize, min(playerPos->first, map->row - 1 - smallMapSize));
  int ycenter = max(smallMapSize, min(playerPos->second, map->col - 1 - smallMapSize));

  for (int i = xcenter - smallMapSize; i <= xcenter + smallMapSize; i++) {
    for (int j = ycenter - smallMapSize; j <= ycenter + smallMapSize; j++) {
      if (playerPos->first == i && playerPos->second == j) {
        printf(YEL "%c " RESET, 'P');
      } else if (map->data[i][j] == '@') {
        printf("%c ", '@');
      } else if (map->data[i][j] == '9') {
        printf("%c ", ' ');
      } else {
        if (watchTowerCnt) {
          switch (map->data[i][j]) {
            case '0':
              printf(GRN "%c " RESET, 'H');
              break;
            case '1':
              printf(RED "%c " RESET, 'H');
              break;
            case '2':
              printf(GRN "%c " RESET, 'A');
              break;
            case '3':
              printf(RED "%c " RESET, 'A');
              break;
            case '4':
              printf(GRN "%c " RESET, 'D');
              break;
            case '5':
              printf(RED "%c " RESET, 'D');
              break;
            case '6':
              printf(MAG "%c " RESET, 'M');
              break;
            case '7':
              printf(MAG "%c " RESET, 'W');
              break;
            case '8':
              printf(MAG "%c " RESET, 'I');
              break;
            default:
              printf("%c ", ' ');
              break;
          }
          // printf("%c ", map->data[i][j]);
        } else {
          printf("%c ", ' ');
        }
      }
    }
    printf("\n");
    printf("\e[%d;%dH", ++y, x);
  }
}

void chooseItem(PlayerData *p, Game *g) {
  int choice = 0;
  char ch;
  drawBackpack(p, g, 0, win_row - TEXT_AREA_HEIGHT + 3, (win_col - MAP_AREA_WIDTH) / 2 - 35);
  printf(HIDE_CURSOR);

  while (true) {
    start = clock();
    ssize_t bytesRead = read(STDIN_FILENO, &ch, 1);
    clearInputBuffer();

    if (bytesRead == 1) {
      if (ch == 'a') {
        choice = (choice - 1 + sizeof(items_ratio) / sizeof(float)) % (sizeof(items_ratio) / sizeof(float));
      } else if (ch == 'd') {
        choice = (choice + 1) % (sizeof(items_ratio) / sizeof(float));
      } else if (ch == '\n') {
        if (p->backpack[choice] && (g->status == 2 && items_maze_usability[choice] || g->status == 3 && !items_maze_usability[choice])) {
          g->items_enabled[choice] = !g->items_enabled[choice];
        }
      } else if (ch == 'q') {
        break;
      } else {
        end = clock();
        one_tick(start, end);
        continue;
      }
    } else {
      end = clock();
      one_tick(start, end);
      continue;
    }

    drawBackpack(p, g, choice, win_row - TEXT_AREA_HEIGHT + 3, (win_col - MAP_AREA_WIDTH) / 2 - 35);
    printf(HIDE_CURSOR);
  }
}

void drawMessage(Map *m, IntPair *playerPos, PlayerData *p, Game *game, int y, int x) {

  printf("\e[%d;%dH", y, x);
  switch (game->event) {
    case '0':
      printf("you gain hp!");
      break;
    case '1':
      printf("you loose hp!");
      break;
    case '2':
      printf("you gain atk!");
      break;
    case '3':
      printf("you loose atk!");
      break;
    case '4':
      printf("you gain atk!");
      break;
    case '5':
      printf("you loose atk!");
      break;
    case '6':
      printf("you encounter the monster!");
      break;
    case '7':
      printf("you can see the all the map!");
      break;
    case '8':
      printf("you gain %s x1!", items_name[game->gained_item]);
      break;
    case 'B':
      printf("you encounter the boss!");
    default:
      break;
  }
}

#endif