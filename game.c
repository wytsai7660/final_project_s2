// #include "3d_renderer"
// #include "battle.c"
#include "draw.c"
// #include "enemy.c"
#include "header.h"
#include "types.h"

void chooseItem(PlayerData *p, Game *g) {
  int choice = 0;
  char ch;
  drawBackpack(p, g, 0, TEXT_AREA_HEIGHT / 3, (win_col - MAP_AREA_WIDTH) / 2 - 35);

  while (ch = getchar())
  {
    if (ch == 'a') {
      choice = (choice - 1 + sizeof(items_ratio)/sizeof(float)) % (sizeof(items_ratio)/sizeof(float));
    } else if (ch == 'd') {
      choice = (choice + 1) % (sizeof(items_ratio)/sizeof(float));
    } else if (ch == '\n') {
      if(p->backpack[choice] && items_maze_usability[choice]) g->items_enabled[choice] = !g->items_enabled[choice];
    } else if (ch == 'q') {
      break;
    }

    drawBackpack(p, g, choice, TEXT_AREA_HEIGHT / 3, (win_col - MAP_AREA_WIDTH) / 2 - 35);
  }
}

int main() {
  srand((unsigned)time(NULL));
  // obtain the terminal window's size (row and column) 
#ifdef __linux__
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  win_row = w.ws_row, win_col = w.ws_col;
#elif _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  win_row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1, win_col = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#endif

  // setting the cursor
  struct termios old_attr, new_attr;
  tcgetattr(STDIN_FILENO, &old_attr);
  new_attr = old_attr;
  new_attr.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);

  // variable init
  Map *map = new_Map(MAP_ROW, MAP_COL);
  PlayerData *player = new_PlayerData();
  Game *game = new_Game();
  char ch;
  const int smallMapSize = 7;
  game->status = 2;

  gen_maze(map);
  for (int i = 0; i < map->row; i++) {
      for (int j = 0; j < map->col; j++) {
          if(map->data[i][j] == 'P'){
              player->pos = make_IntPair(i, j);
          }
      } 
  }

  //game loop
  while(ch = getchar()) {

    printf(HIDE_CURSOR);



    switch (ch) {
      case 'w':
          printf(CLEAR);
          player->dir = 2;
          break;
      case 'a':
          printf(CLEAR);
          player->dir = 1;
          break;
      case 's':
          printf(CLEAR);
          player->dir = 0;
          break;
      case 'd':
          printf(CLEAR);
          player->dir = 3;
          break;
      case 'e':
          chooseItem(player, game);
          printf(CLEAR);
          break;
      default:
          continue;
    }
    
    drawBox(TEXT_AREA_HEIGHT, win_col - MAP_AREA_WIDTH, 1, 1);
    drawBox(TEXT_AREA_HEIGHT, MAP_AREA_WIDTH, 1, win_col - MAP_AREA_WIDTH + 1);
    printf("\e[%d;%dH", 2, 3);
    printf("[W] To Move   [A][D] To Turn   [E] To Open Backpack");

    if (!(map->data[player->pos.first + direction[player->dir][0]][player->pos.second + direction[player->dir][1]] == '@') && ch != 'e')
    {
      player->watchTowerCnt -= player->watchTowerCnt ? 1 : 0;
      player->pos.first += direction[player->dir][0];
      player->pos.second += direction[player->dir][1];
      playerEvent(map, &player->pos, player, game, 4, 3);
    }

    if(game->items_enabled[0]) {
      int y, x;
      printf("\e[%d;%dH", 5, 3);
      do {
        y = rand_between(0, MAP_ROW);
        x = rand_between(0, MAP_COL);
      } while(map->data[y][x] == '@');

      printf("teleport to coordinate: %d, %d", y, x);
      player->pos.first = y, player->pos.second = x;
      game->items_enabled[0] = false;
    }
    if(game->items_enabled[3]) {
      player->watchTowerCnt = 10;
      game->items_enabled[3] = false;
    }

    drawMiniMap(map, &player->pos, smallMapSize, player->watchTowerCnt, 2, win_col - MAP_AREA_WIDTH + 3);
    drawStatusBar(player, win_col - MAP_AREA_WIDTH, TEXT_AREA_HEIGHT - 1, 3);
    printf("\e[%d;%dH", win_row, 1);
    // delay(0.03);
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);
}