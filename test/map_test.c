#include "../map.c"

int main() {
  srand((unsigned)time(NULL));
  Map *map = new_Map(17, 101);
  gen_maze(map);
  for (unsigned i = 0; i < map->row; i++) {
    for (unsigned j = 0; j < map->col; j++) printf("%c ", map->data[i][j]);
    printf("\n");
  }
  printf("%d", map->path);
  Map_clear(map);
}