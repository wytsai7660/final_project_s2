#include "../map.c"

int main() {
  srand((unsigned)time(NULL));
  Map *map = new_Map(17, 17);
  int arr[10] = {};
  gen_maze(map);
  for (int i = 0; i < map->row; i++) {
    for (int j = 0; j < map->col; j++) {
      printf("%c ", map->data[i][j]);
      if (map->data[i][j] >= '0' && map->data[i][j] <= '9') arr[map->data[i][j] - '0']++;
    }
    printf("\n");
  }
  printf("\n\n");
  for (int i = 0; i < map->row; i++) {
    for (int j = 0; j < map->col; j++) printf("%c ", map->data[i][j] == '@' ? '@' : ' ');
    printf("\n");
  }
  printf("%d\n", map->path);
  for (int i = 0; i < 10; i++) printf("%d: %d\n", i, arr[i]);
  Map_clear(map);
}