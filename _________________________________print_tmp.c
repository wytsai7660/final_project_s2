void print_2d_maze(int row, int col, maze map) {
  int r = row - map.size - 2, c = col - 2 * map.size - 4;
  printf("\033[%d;%dH\033[1;30m +", r, c);
  for (int i = 0; i < map.size * 2 + 1; i++) printf("-");
  printf("+ \033[0m");
  for (int i = 0; i < map.size; i++) {
    printf("\033[%d;%dH\033[1;30m |\033[0m ", r + i + 1, c);
    for (int j = 0; j < map.size; j++) printf("\33[%s%s%dm%c\33[0m ", (map.data[j][i] == ' ' || map.data[j][i] == '@') ? "" : "1;", (map.data[j][i] == ' ' || map.data[j][i] == '@' || map.data[j][i] == 'P') ? "" : "3;5;", (map.data[j][i] == ' ' || map.data[j][i] == '@') ? 0 : (int)map.data[j][i] / 10 + 26, map.data[j][i]);
    printf("\033[1;30m|\033[0m ");
  }
  printf("\033[%d;%dH\033[1;30m +", r + map.size + 1, c);
  for (int i = 0; i < map.size * 2 + 1; i++) printf("-");
  printf("+ \033[0m");
}

void new_text(int row, int col) {
  printf("\033[%d;%dH", row - text_area_height, 0);
  for (int i = 0; i < col; i++) printf("-");
  printf("\n\033[J");
}

void clear_text(int row, int col, maze map) {
  int r = row - map.size - 2, c = col - 2 * map.size - 5;
  for (int i = row - text_area_height - r + 1; r + i <= row; i++) printf("\033[%d;%dH\033[1K", r + i, c /*, r + i, col - 1*/);  //\033[%d;%dH\033[K
  printf("\033[%d;%dH", row - text_area_height + 1, 0);
}

//    money: 1540      |  time to make a hotdog: 15        |  price of a hotdog: 30        |  free lottery choice: 1         |  you are at maze: 1         |  [S][P][A]
void print_stat(int row, int col, int money, int time, int price, int choice, int maze_num, queue boost_q, int boost_open[], maze map) {
  printf("\033[H");
  // money, time to make a hotdog, price of a hotdog
  for (int i = 0; i < (int)((float)col / 2 - 161.0 / 2); i++) printf(" ");
  printf("money: \033[1;34m%-10d\033[0m|  time to make a hotdog: \033[1;34m%-10d\033[0m|  price of a hotdog: \033[1;34m%-10d\033[0m|  free lottery choice%c: \033[1;34m%-10d\033[0m|  you are at maze: \033[1;34m%-10d\033[0m|  \033[1;%sm[S]\033[0m\033[1;%sm[P]\033[0m\033[1;%sm[A]\033[0m\033[K\n", money, time, price, choice > 2 ? 's' : '\0', choice, maze_num, boost_open[0] ? "" : "30", boost_open[1] ? "" : "30", boost_open[2] ? "" : "30");
  for (int i = 0; i < (int)((float)col / 2 - (float)(boost_q.length > 0 ? boost_q.length * 3 : 33) / 2); i++) printf(" ");
  printf("\033[1;33m[%c%s]\033[0m", boost_q.length > 0 ? queue_front(&boost_q) : '\0', boost_q.length > 0 ? "" : "you don't have any booster yet!");
  for (int i = 1; i < boost_q.length; i++) printf("[%c]", boost_q.data[(boost_q.front + i) % boost_q.max_slots]);
  printf("\033[K");
  print_2d_maze(row, col, map);
  // for (int i = 0; i < (int)((float)col / 2 - (float)(boost_q.length > 0 ? boost_q.length * 3 : 33) / 2); i++) printf(" ");
  // printf("\n%d    ", boost_q.front);
  // for (int i = 0; i < boost_q.max_slots; i++) printf("%c ", boost_q.data[(boost_q.front + i) % boost_q.max_slots]);
  // clear_text(row, col);
  printf("\033[%d;%dH", row - text_area_height + 1, 0);
}