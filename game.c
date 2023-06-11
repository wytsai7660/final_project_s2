// #include "3d_renderer"
#include "battle.c"
#include "draw.c"
#include "map.c"
#include "header.h"
#include "types.h"


void playerEvent(Map *m, IntPair *playerPos, PlayerData *p, Game *game, int y, int x){
    char ch = m->data[playerPos->first][playerPos->second];
    if (ch != '7') m->data[playerPos->first][playerPos->second] = '9';
    int item;
    
    printf("\e[%d;%dH", y, x);
    switch (ch)
    {
    case '0':
        printf("you gain hp!");
        p->max_hp++;
        break;
    case '1':
        printf("you loose hp!");
        p->max_hp = max(p->max_hp-1, 1);
        break;
    case '2':
        printf("you gain atk!");
        p->atk++;
        break;
    case '3':
        printf("you loose atk!");
        p->atk = max(p->atk-1, 1);
        break;
    case '4':
        printf("you gain atk!");
        p->def++;
        break;
    case '5':
        printf("you loose atk!");
        p->def = max(p->def-1, 0);
        break;
    case '6':
        printf("you encounter the monster!");
        game->status = 3;
        break;
    case '7':
        printf("you can see the all the map!");
        p->watchTowerCnt += 1;
        break;
    case '8':
        item = sample(items_ratio, sizeof(items_ratio)/sizeof(items_ratio[0]));
        printf("you gain %s x1!", items_name[item]);
        p->backpack[item]++;
        break;
    case 'B':
        printf("you encounter the boss!");
        game->is_boss = true;
        game->status = 3;
    default:
        break;
    }
}

void spawnBoss(Map *m) {
  int x=0, y=0;
  while(m->data[y][x] == '@' || m->data[y][x] == 'P') {
    y = rand_between(0, MAP_ROW);
    x = rand_between(0, MAP_COL);
  }
  m->data[y][x] = 'B';
}

void mapLoop(Game *game, PlayerData *player, Map *map) {
  char ch;

  while(game->status == 2) {
    start = clock();

    ssize_t bytesRead = read(STDIN_FILENO, &ch, 1);
    clearInputBuffer();
    if(bytesRead == 1)
    {
      switch (ch) {
        case 'w':
            printf(CLEAR HIDE_CURSOR);
            player->dir = 2;
            break;
        case 'a':
            printf(CLEAR HIDE_CURSOR);
            player->dir = 1;
            break;
        case 's':
            printf(CLEAR HIDE_CURSOR);
            player->dir = 0;
            break;
        case 'd':
            printf(CLEAR HIDE_CURSOR);
            player->dir = 3;
            break;
        case 'e':
            chooseItem(player, game);
            printf(CLEAR HIDE_CURSOR);
            break;
        default:
          end = clock();
          one_tick(start, end);
          continue;
      }
    } 
    else {
      end = clock();
      one_tick(start, end);
      continue;
    }

    drawBox(TEXT_AREA_HEIGHT, win_col - MAP_AREA_WIDTH, win_row - TEXT_AREA_HEIGHT - 1, 1);
    drawBox(TEXT_AREA_HEIGHT, MAP_AREA_WIDTH, win_row - TEXT_AREA_HEIGHT - 1, win_col - MAP_AREA_WIDTH + 1);
    printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT, 3);
    printf("[W] To Move   [A][D] To Turn   [E] To Open Backpack");

    if (!(map->data[player->pos.first + direction[player->dir][0]][player->pos.second + direction[player->dir][1]] == '@') && ch != 'e')
    {
      player->watchTowerCnt -= player->watchTowerCnt ? 1 : 0;
      player->pos.first += direction[player->dir][0];
      player->pos.second += direction[player->dir][1];
      playerEvent(map, &player->pos, player, game, win_row - TEXT_AREA_HEIGHT + 2, 3);
      game->round++;
      if(game->round == 35) spawnBoss(map);
    }

    if(game->items_enabled[0]) {
      int y, x;
      printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT + 3, 3);
      do {
        y = rand_between(0, MAP_ROW);
        x = rand_between(0, MAP_COL);
      } while(map->data[y][x] == '@');

      printf("teleport to coordinate: %d, %d", y, x);
      player->pos.first = y, player->pos.second = x;
      player->backpack[0]--;
      game->items_enabled[0] = false;
    }
    if(game->items_enabled[3]) {
      player->watchTowerCnt += 10;
      player->backpack[3]--;
      game->items_enabled[3] = false;
    }

    drawMiniMap(map, &player->pos, MINIMAP_SIZE, player->watchTowerCnt, win_row - TEXT_AREA_HEIGHT, win_col - MAP_AREA_WIDTH + 3);
    drawStatusBar(player, true, win_row - 3, 3);

    end = clock();
    one_tick(start, end);
  }
}

int main() {
  srand((unsigned)time(NULL));
  // obtain the terminal window's size (row and column) 
#ifdef __linux__
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  win_row = w.ws_row, win_col = w.ws_col;

  // setting the cursor
  struct termios term;
  struct termios old_term;
  tcgetattr(STDIN_FILENO, &old_term);
  term = old_term;
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
  Game *game = new_Game();

  gen_maze(map);
  for (int i = 0; i < map->row; i++) {
    for (int j = 0; j < map->col; j++) {
      if (map->data[i][j] == 'P') {
        player->pos = make_IntPair(i, j);
      }
    }
  }

  game->status = 0;

  //game loop
  while(true) {
    if(game->status == 0) 
    {
      printf(CLEAR);
      Animation *you_win = new_Animation("assets/you_win.txt"); // https://fsymbols.com/generators/carty/
      if(you_win == NULL) return -1;
      drawAnimation(you_win, 0, win_row/2 - you_win->row/2, win_col/2 - you_win->col/6);
      Animation_clear(you_win);
      printf("\e[%d;%dH", win_row/2 + 5, win_col/2 - 12);
      printf("[PRESS ANY KEY TO START]");
      getchar();
      game->status = 2;
    }
    else if(game->status == 2) 
    {
      mapLoop(game, player, map);
    }
    else if(game->status == 3) 
    {
      battleLoop(game, player, map);
    }
    else if(game->status == 8) 
    {
      delay(3);
      printf(CLEAR);
      Animation *you_win = new_Animation("assets/you_win.txt"); // https://fsymbols.com/generators/carty/
      if(you_win == NULL) return -1;
      drawAnimation(you_win, 0, win_row/2 - you_win->row/2, win_col/2 - you_win->col/6);
      Animation_clear(you_win);
      printf("\e[%d;%dH", win_row - 1, 1);
      break;
    }
    else if(game->status == 9) 
    {
      delay(3);
      printf(CLEAR);
      Animation *game_over = new_Animation("assets/game_over.txt"); // https://fsymbols.com/generators/carty/
      if(game_over == NULL) return -1;
      drawAnimation(game_over, 0, win_row/2 - game_over->row/2, win_col/2 - game_over->col/6);
      Animation_clear(game_over);
      printf("\e[%d;%dH", win_row - 1, 1);
      break;
    }
    else 
    {
      printf("wrong game status: %d\n", game->status);
      return -1;
    }

  }

  // free var
  PlayerData_clear(player);
  Game_clear(game);
  Map_clear(map);
  tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
}