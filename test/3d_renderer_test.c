#include "../3d_renderer.c"

#include "../header.h"
#include "../map.c"

int main() {
  srand((unsigned)time(NULL));
  Map *map = new_Map(17, 17);
  int arr[10] = {};
  IntPair tmp = gen_maze(map);
  printf("gererated\n");
  for (int i = 0; i < map->row; i++) {
    for (int j = 0; j < map->col; j++) {
      printf("%c ", map->data[i][j]);
      if (map->data[i][j] >= '0' && map->data[i][j] <= '9') arr[map->data[i][j] - '0']++;
    }
    printf("\n");
  }
  FloatPair pos = make_FloatPair((float)tmp.first, (float)tmp.second);
  printf("pos: %f %f\n", pos.first, pos.second);
  render(*map, pos, .0f);

  //   for (float dir = 0.f; dir <= 1000.f; dir += rotate_spacing) {
  //     render(*map, pos, fmodf(dir, 4.f));
  //     printf("%f\n", dir);
  //     delay(.5f);
  //   }

  //   printf("\n\n");
  //   for (int i = 0; i < map->row; i++) {
  //     for (int j = 0; j < map->col; j++) printf("%c ", map->data[i][j] == '@' ? '@' : ' ');
  //     printf("\n");
  //   }
  //   printf("%d\n", map->path);
  //   for (int i = 0; i < 10; i++) printf("%d: %d\n", i, arr[i]);
  Map_clear(map);
}