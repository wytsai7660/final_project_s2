// #include "3d_renderer"
// #include "battle.c"
#include "draw.c"
// #include "enemy.c"
#include "header.h"
#include "types.h"

void chooseItem(PlayerData *player) {
  int choice = 0;
  char ch;
  while (ch = getchar())
  {
    drawBackpack(player, 0, TEXT_AREA_HEIGHT / 3, (win_col - MAP_AREA_WIDTH) / 2 - 35);
    if (ch == 'a') {
      choice = max(choice - 1, 0);
    } else if (ch == 'd') {
      choice = min(choice + 1, sizeof(items_ratio)/sizeof(float));
    } else if (ch == '\n') {
      
    } else if (ch == 'q') {
      break;
    } else {
      continue;
    }
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

    printf("\e[1;1H\e[2J");
    printf("\e[?25l");



    switch (ch) {
      case 'w':
          player->dir = 2;
          break;
      case 'a':
          player->dir = 1;
          break;
      case 's':
          player->dir = 0;
          break;
      case 'd':
          player->dir = 3;
          break;
      case 'e':
          break;
      default:
          continue;
    }
    
    drawBox(TEXT_AREA_HEIGHT, win_col - MAP_AREA_WIDTH, 1, 1);
    drawBox(TEXT_AREA_HEIGHT, MAP_AREA_WIDTH, 1, win_col - MAP_AREA_WIDTH + 1);
    printf("\e[%d;%dH", 2, 3);
    printf("[W] To Move   [A][D] To Turn   [E] To Open Backpack");

    if (!(map->data[player->pos.first + direction[player->dir][0]][player->pos.second + direction[player->dir][1]] == '@'))
    {
      player->watchTowerCnt -= player->watchTowerCnt ? 1 : 0;
      player->pos.first += direction[player->dir][0];
      player->pos.second += direction[player->dir][1];
      playerEvent(map, &player->pos, player, game, 4, 3);
    }

    drawMiniMap(map, &player->pos, smallMapSize, player->watchTowerCnt, 2, win_col - MAP_AREA_WIDTH + 3);
    
    // drawChoice(1, 3, 3);
    // drawHp(player->hp, 15, 10, 3);
    printf("\n");
    drawBackpack(player, 0, TEXT_AREA_HEIGHT / 3, (win_col - MAP_AREA_WIDTH) / 2 - 35);
    drawStatusBar(player, win_col - MAP_AREA_WIDTH, TEXT_AREA_HEIGHT - 1, 3);
    printf("\n");
    printf("\e[%d;%dH", 19, 1);
    // delay(0.03);
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);
}