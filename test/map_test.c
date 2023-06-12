#include "../map.c"

int main() {
  srand((unsigned)time(NULL));
  Map *map = new_Map(17, 17);
  int arr[10] = {};
  IntPair p = gen_maze(map);
  // for (int i = 0; i < map->row; i++) {
  //   for (int j = 0; j < map->col; j++) {
  //     printf("%c ", map->data[i][j]);
  //     if (map->data[i][j] >= '0' && map->data[i][j] <= '9') arr[map->data[i][j] - '0']++;
  //   }
  //   printf("\n");
  // }
  // printf("\n\n");
  printf(CLEAR);
  for (int i = 0; i < map->row; i++) {
    for (int j = 0; j < map->col; j++) printf("%c ", map->data[i][j] == '@' ? '@' : ' ');
    printf("\n");
  }
  // printf("%d\n", map->path);
  // for (int i = 0; i < 10; i++) printf("%d: %d\n", i, arr[i]);

  // for (int i = 0; i < map->row; i++) {
  //   for (int j = 0; j < map->col; j++) {
  //     if (p.first == i && p.second == j) {
  //       printf(YEL "%c " RESET, 'P');
  //     } else if (map->data[i][j] == '@') {
  //       printf("%c ", '@');
  //     } else if (map->data[i][j] == '9') {
  //       printf("%c ", ' ');
  //     } else {
  //       switch (map->data[i][j]) {
  //         case '0':
  //           printf(GRN "%c " RESET, 'H');
  //           break;
  //         case '1':
  //           printf(RED "%c " RESET, 'H');
  //           break;
  //         case '2':
  //           printf(GRN "%c " RESET, 'A');
  //           break;
  //         case '3':
  //           printf(RED "%c " RESET, 'A');
  //           break;
  //         case '4':
  //           printf(GRN "%c " RESET, 'D');
  //           break;
  //         case '5':
  //           printf(RED "%c " RESET, 'D');
  //           break;
  //         case '6':
  //           printf(MAG "%c " RESET, 'M');
  //           break;
  //         case '7':
  //           printf(MAG "%c " RESET, 'W');
  //           break;
  //         case '8':
  //           printf(MAG "%c " RESET, 'I');
  //           break;
  //         default:
  //           printf("%c ", ' ');
  //           break;
  //       }
  //     }
  //   }
  //   printf("\n");
  // }
  printf("\n");
  Map_clear(map);
}