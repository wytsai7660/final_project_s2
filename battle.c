#include "header.h"
#include "types.h"
#include "draw.c"
#include "map.c"

const char *moves[] = {"Paper", "Scissor", "Stone"};

void clearPanel() {
	for(int i= win_row - TEXT_AREA_HEIGHT;i< win_row; i++){
		printf("\e[K\e[B");
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
      if (map->data[i][j] == 'P') {
        player->pos = make_IntPair(i, j);
      }
    }
  }

  //game loop
  while(ch = getchar()) {

    printf(HIDE_CURSOR);

    clearPanel();


    drawBox(TEXT_AREA_HEIGHT, win_col - MAP_AREA_WIDTH, win_row - TEXT_AREA_HEIGHT, 1);
    drawBox(TEXT_AREA_HEIGHT, MAP_AREA_WIDTH, win_row - TEXT_AREA_HEIGHT, win_col - MAP_AREA_WIDTH + 1);
    printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT + 1, 3);
    printf("[A][D] To Select   [ENTER] To Choose   [E] To Open Backpack");
    printf("\e[%d;%dH", win_row, 1);
    delay(0.03);
  }

  // free var
  PlayerData_clear(player);
  Game_clear(game);
  Map_clear(map);
  tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);
}