#include "3d_renderer.c"
#include "battle.c"
#include "draw.c"
#include "header.h"
#include "map.c"
#include "types.h"

// #define DEMO

void playerEvent(Map *m, IntPair *playerPos, PlayerData *p, Game *game, int y, int x) {
  char ch = m->data[playerPos->first][playerPos->second];
  game->event = ch;
  if (ch != '7') m->data[playerPos->first][playerPos->second] = '9';

  printf("\e[%d;%dH", y, x);
  switch (ch) {
    case '0':
      p->max_hp++;
      break;
    case '1':
      p->max_hp = max(p->max_hp - 1, 1);
      break;
    case '2':
      p->atk++;
      break;
    case '3':
      p->atk = max(p->atk - 1, 1);
      break;
    case '4':
      p->def++;
      break;
    case '5':
      p->def = max(p->def - 1, 0);
      break;
    case '6':
      game->status = 3;
      break;
    case '7':
      p->watchTowerCnt += 1;
      break;
    case '8':
      game->gained_item = sample(items_ratio, sizeof(items_ratio) / sizeof(items_ratio[0]));
      p->backpack[game->gained_item]++;
      break;
    case 'B':
      game->is_boss = true;
      game->status = 3;
    default:
      break;
  }
}

void spawnBoss(Map *m) {
  int x = 0, y = 0;
  while (m->data[y][x] == '@' || m->data[y][x] == 'P') {
    y = rand_between(0, MAP_ROW);
    x = rand_between(0, MAP_COL);
  }
  m->data[y][x] = 'B';
}

void drawPanel(Map *map, PlayerData *player, Game *game) {
  drawBox(TEXT_AREA_HEIGHT, win_col - MAP_AREA_WIDTH, win_row - TEXT_AREA_HEIGHT, 1);
  drawBox(TEXT_AREA_HEIGHT, MAP_AREA_WIDTH, win_row - TEXT_AREA_HEIGHT, win_col - MAP_AREA_WIDTH + 1);
  printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT + 1, 3);
  printf("[W] To Move   [A][D] To Turn   [E] To Open Backpack");
  drawMessage(game, win_row - TEXT_AREA_HEIGHT + 3, 3);
  drawMiniMap(map, &player->pos, MINIMAP_SIZE, player->watchTowerCnt, win_row - TEXT_AREA_HEIGHT + 1, win_col - MAP_AREA_WIDTH + 3);
  drawStatusBar(player, true, win_row - 2, 3);
}

void mapLoop(Game *game, PlayerData *player, Map *map) {
  char ch;
  int current_tick;
  bool updateAnimationOnly;
  game->input_locked = false;
  IntPair old_pos = make_IntPair(player->pos.first, player->pos.second);
  int old_dir;

  printf(CLEAR);
  render(*map, make_FloatPair((float)player->pos.second, (float)player->pos.first), (float)player->dir);
  drawPanel(map, player, game);
  while (game->status == 2 || game->input_locked) {
    start = clock();

    updateAnimationOnly = false;
    ssize_t bytesRead = read(STDIN_FILENO, &ch, 1);
    clearInputBuffer();
    old_dir = player->dir;
    ch = (char)toupper(ch);
    if (bytesRead == 1 && !game->input_locked) {
      switch (ch) {
        case 'W':
          break;
        case 'A':
          player->dir = (player->dir + 1) % 4;
          game->input_locked = true;
          current_tick = tick;
          updateAnimationOnly = true;
          break;
        case 'D':
          player->dir = (player->dir - 1 + 4) % 4;
          game->input_locked = true;
          current_tick = tick;
          updateAnimationOnly = true;
          break;
        case 'E':
          chooseItem(player, game);
          break;
        default:
          updateAnimationOnly = true;
      }
    } else {
      updateAnimationOnly = true;
    }

    // update 3d_renderer
    if (game->input_locked) {
      int delta_tick = (tick - current_tick);
      if (delta_tick <= 10) {
        printf(CLEAR);

        if (ch == 'W') {
          render(*map, make_FloatPair((float)(old_pos.second + (player->pos.second - old_pos.second) * delta_tick / 10.0), (float)(old_pos.first + (player->pos.first - old_pos.first) * delta_tick / 10.0)), fmodf((float)old_dir * (1.f - (float)delta_tick / 10.f) + (float)player->dir * (float)delta_tick / 10.f, 4.f));
        } else {
          if (ch == 'A') {
            render(*map, make_FloatPair((float)player->pos.second, (float)player->pos.first), (float)((player->dir + 3) % 4) + (float)delta_tick / 10.f);
          }
          if (ch == 'D') {
            render(*map, make_FloatPair((float)player->pos.second, (float)player->pos.first), (float)((player->dir + 1) % 4) - (float)delta_tick / 10.f);
          }
        }

        drawPanel(map, player, game);

      } else {
        drawPanel(map, player, game);
        game->input_locked = false;
      }
    }

    if (updateAnimationOnly) {
      end = clock();
      one_tick(start, end);
      continue;
    }

    if (toupper(ch) == 'W' && !(map->data[player->pos.first + direction[player->dir][1]][player->pos.second + direction[player->dir][0]] == '@')) {
      player->watchTowerCnt -= player->watchTowerCnt ? 1 : 0;
      old_pos.first = player->pos.first, old_pos.second = player->pos.second;
      player->pos.first += direction[player->dir][1];
      player->pos.second += direction[player->dir][0];
      playerEvent(map, &player->pos, player, game, win_row - TEXT_AREA_HEIGHT + 2, 3);

      game->round++;
      if (game->round == 35) spawnBoss(map);
      game->input_locked = true;
      current_tick = tick + 1;
    }

    int y, x;
    if (game->items_enabled[0]) {
      do {
        y = rand_between(0, MAP_ROW);
        x = rand_between(0, MAP_COL);
      } while (map->data[y][x] == '@');
      player->pos.first = y, player->pos.second = x;

      render(*map, make_FloatPair((float)player->pos.second, (float)player->pos.first), (float)player->dir);
      drawPanel(map, player, game);

      printf("\e[%d;%dH", win_row - TEXT_AREA_HEIGHT + 3, 3);
      printf("teleport to coordinate: %d, %d", y, x);

      player->backpack[0]--;
      game->items_enabled[0] = false;
    } else if (game->items_enabled[3]) {
      player->watchTowerCnt += 10;
      drawPanel(map, player, game);
      player->backpack[3]--;
      game->items_enabled[3] = false;
    } else {
      drawPanel(map, player, game);
    }

    render(*map, make_FloatPair((float)player->pos.second, (float)player->pos.first), (float)player->dir);
    drawPanel(map, player, game);

    end = clock();
    one_tick(start, end);
  }

  delay(1.5f);
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
  term.c_cc[VTIME] = 0;  // Set the inter-character timer to 0
  term.c_cc[VMIN] = 1;   // Wait for at least 1 character before reading
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
  player->pos = gen_maze(map);
  char ch;

  game->status = 0;

#ifdef DEMO
  game->status = 0;
  game->is_boss = true;
  player->backpack[0] = 5;
  player->backpack[1] = 5;
  player->backpack[2] = 5;
  player->backpack[3] = 5;
  player->atk = 50;
#endif

  // game loop
  while (true) {
    if (game->status == 0) {
      printf(CLEAR);
      Animation *dungeon = new_Animation("assets/dungeon.txt");  // https://fsymbols.com/generators/carty/
      if (dungeon == NULL) return -1;
      drawAnimation(dungeon, 0, win_row / 2 - dungeon->row / 2, win_col / 2 - dungeon->col / 6);
      Animation_clear(dungeon);
      printf("\e[%d;%dH", win_row / 2 + 5, win_col / 2 - 12);
      printf("[PRESS ENTER TO START]");
      while ((ch = (char)getchar()) != '\n')
        ;
      game->status = 2;
    } else if (game->status == 2) {
      mapLoop(game, player, map);
    } else if (game->status == 3) {
      // game->status = 2;
      battleLoop(game, player, map);
    } else if (game->status == 8) {
      delay(3);
      printf(CLEAR);
      Animation *you_win = new_Animation("assets/you_win.txt");  // https://fsymbols.com/generators/carty/
      if (you_win == NULL) return -1;
      drawAnimation(you_win, 0, win_row / 2 - you_win->row / 2, win_col / 2 - you_win->col / 6);
      Animation_clear(you_win);
      printf("\e[%d;%dH", win_row - 1, 1);
      break;
    } else if (game->status == 9) {
      delay(3);
      printf(CLEAR);
      Animation *goat = new_Animation("assets/goat.txt");
      Animation *game_over = new_Animation("assets/game_over.txt");  // https://fsymbols.com/generators/carty/
      if (goat == NULL) return -1;
      if (game_over == NULL) return -1;
      drawAnimation(goat, 0, 1, 1);
      drawAnimation(game_over, 0, win_row / 2 - game_over->row / 2, win_col / 2 - game_over->col / 6);
      Animation_clear(game_over);
      Animation_clear(goat);
      printf("\e[%d;%dH", win_row - 1, 1);
      break;
    } else {
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