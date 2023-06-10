#include "header.h"
#include "types.h"
#include "draw.c"
#include "map.c"

char *moves[] = {"Paper", "Scissor", "Stone"};

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

  struct termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag &= ~(ICANON | ECHO);
  term.c_cc[VTIME] = 0; // Set the inter-character timer to 0
  term.c_cc[VMIN] = 1; // Wait for at least 1 character before reading
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
#elif _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  win_row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1, win_col = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#endif

  // variable init
  Map *map = new_Map(MAP_ROW, MAP_COL);
  PlayerData *player = new_PlayerData();
  Enemy *enemy = new_Enemy(player, 5);
  Game *game = new_Game();
  game->status = 3;
  clock_t start, end;
  double cpu_time_used;
  char ch;
  int choice = 0, chosen = -1;
  double fps;
  player->hp = player->hpMax;

  gen_maze(map);
  for (int i = 0; i < map->row; i++) {
    for (int j = 0; j < map->col; j++) {
      if (map->data[i][j] == 'P') {
        player->pos = make_IntPair(i, j);
      }
    }
  }

  //game loop
  while(game->status == 3) {
    
    start = clock();

    //clearPanel();
    ssize_t bytesRead = read(STDIN_FILENO, &ch, 1);
    clearInputBuffer();

    if(bytesRead == 1) {
      if(toupper(ch) == 'A') {
        printf(CLEAR);
        choice = (choice - 1 + 3)%3;
      }
      else if(toupper(ch) == 'D') {
        printf(CLEAR);
        choice = (choice + 1)%3;
      }
      else if(ch == '\n') {
        printf(CLEAR);
        chosen = choice;
      }
      else if(toupper(ch) == 'E') {
        clearPanel();
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

    drawSolidBox(13, 25, 0, 9, win_col / 4);
    printf("\e[%d;%dH", 9 + 14, win_col / 4 - 12);
    drawHp(player->hp, player->hpMax);
    drawSolidBox(13, 25, 0, 5, win_col / 4 * 3);
    printf("\e[%d;%dH", 5 + 14, win_col / 4 * 3 - 12);
    drawHp(enemy->hp, enemy->hpMax);


    drawBox(TEXT_AREA_HEIGHT, win_col - MAP_AREA_WIDTH, win_row - TEXT_AREA_HEIGHT - 1, 1);
    drawBox(TEXT_AREA_HEIGHT, MAP_AREA_WIDTH, win_row - TEXT_AREA_HEIGHT - 1, win_col - MAP_AREA_WIDTH + 1);
    printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT, 3);
    printf("[A][D] To Select   [ENTER] To Choose   [E] To Open Backpack");
    drawChoice(moves, choice, win_row - TEXT_AREA_HEIGHT + 2, 3);
    if(chosen != -1) {
      printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT + 4, 3);
      printf("You threw %s and enemy threw ***, you win!", moves[chosen]);
      chosen = -1;
    }

    end = clock();
    one_tick(start, end);
  }

  // free var
  PlayerData_clear(player);
  Game_clear(game);
  Map_clear(map);
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}