#include "map.c"
#include "header.h"


void playerEvent(Map *m, IntPair *playerPos, PlayerData *p, Game *game, int y, int x){
    char ch = m->data[playerPos->first][playerPos->second];
    m->data[playerPos->first][playerPos->second] = '9';
    int item;
    
    printf("\e[%d;%dH", y, x);
    switch (ch)
    {
    case '0':
        printf("you gain hp!");
        p->hpMax++;
        break;
    case '1':
        printf("you loose hp!");
        p->hp = max(p->hpMax-1, 1);
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
        // game->status = 3;
        break;
    case '7':
        printf("you can see the all the map!");
        p->watchTowerCnt += 1;
        break;
    case '8':
        printf("you gain some item!");
        item = sample(items_ratio, sizeof(items_ratio)/sizeof(items_ratio[0]));
        p->backpack[item]++;
        break;
    default:
        break;
    }
}

// void mapLoop() {
//     struct termios old_attr, new_attr;
//     tcgetattr(STDIN_FILENO, &old_attr);
//     new_attr = old_attr;
//     new_attr.c_lflag &= ~(ICANON | ECHO);
//     tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);


//     srand((unsigned)time(NULL));
//     Map *map = new_Map(17, 87);
//     PlayerData *player = new_PlayerData();
//     Game *game = new_Game();
//     int smallMapSize = 5;
//     game->status = 2;

//     char ch;

//     gen_maze(map);
//     for (int i = 0; i < map->row; i++) {
//         for (int j = 0; j < map->col; j++) {
//             if(map->data[i][j] == 'P'){
//                 player->pos = make_IntPair(i, j);
//             }
//         } 
//     }

//     // game loop
//     while(game->status == 2 && read(STDIN_FILENO, &ch, 1))
//     {
//         printf("\e[1;1H\e[2J");
//         printf("\e[?25l");
//         printf("hp %d atk %d def %d\n", player->hp, player->atk, player->def);
//         printf("watchTowerCnt %d\n", player->watchTowerCnt);
//         drawMiniMap(map, &player->pos, smallMapSize, player->watchTowerCnt, 1, 1);

        // if(ch == 'w')
        // {
        //     player->dir = 2;
        // }
        // else if(ch == 'a')
        // {
        //     player->dir = 1;
        // }
        // else if(ch == 's')
        // {
        //     player->dir = 0;
        // }
        // else if(ch == 'd')
        // {
        //     player->dir = 3;
        // }else{
        //     continue;
        // }

//         if (!(map->data[player->pos.first + direction[player->dir][0]][player->pos.second + direction[player->dir][1]] == '@'))
//         {
//             player->watchTowerCnt -= player->watchTowerCnt ? 1 : 0;
//             player->pos.first += direction[player->dir][0];
//             player->pos.second += direction[player->dir][1];
//             // IntPairList_push(game->playerPath, (IntPair){player->pos.first, player->pos.second});
//             playerEvent(map, &player->pos, player, game);
//         }
//         // generate new Map events
//         // if list > mapsize**2 / 6
//         // int newMapEvent = sample(events_ratio, sizeof(events_ratio)/sizeof(float));
//         // map->data[][] = newMapEvent + '0';
//         // usleep(30*1000);
//     }
//     tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);
// }


// int main(){
    
// }