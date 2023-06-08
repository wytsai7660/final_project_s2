#include "map.c"
#include "header.h"


void drawMiniMap(Map *map, IntPair *playerPos, int smallMapSize, int watchTowerCnt, int y, int x){
    // printf("player pos %d, %d\n", playerPos->first, playerPos->second);
    printf("\e[%d;%dH", y, x);

    int xcenter = max(smallMapSize, min(playerPos->first, map->row - 1 - smallMapSize));
    int ycenter = max(smallMapSize, min(playerPos->second, map->col - 1 - smallMapSize));

    for(int i = xcenter - smallMapSize; i <= xcenter + smallMapSize; i++){
        for(int j = ycenter - smallMapSize; j <= ycenter + smallMapSize; j++){
            if(playerPos->first == i && playerPos->second == j){
                printf(YEL "%c " RESET, 'P');
            }else if(map->data[i][j] == '@'){
                printf("%c ", '@');
            }else if(map->data[i][j] == '9'){
                printf("%c ", ' ');
            }else{
                if (watchTowerCnt) {
                    switch (map->data[i][j]) {
                        case '0':
                            printf(GRN "%c " RESET, 'H');
                            break;
                        case '1':
                            printf(RED "%c " RESET, 'H');
                            break;
                        case '2':
                            printf(GRN "%c " RESET, 'A');
                            break;
                        case '3':
                            printf(RED "%c " RESET, 'A');
                            break;
                        case '4':
                            printf(GRN "%c " RESET, 'D');
                            break;
                        case '5':
                            printf(RED "%c " RESET, 'D');
                            break;
                        case '6':
                            printf(MAG "%c " RESET, 'M');
                            break;
                        case '7':
                            printf(MAG "%c " RESET, 'W');
                            break;
                        case '8':
                            printf(MAG "%c " RESET, 'I');
                            break;
                        default:
                            printf("%c ", ' ');
                            break;                        
                    }
                    // printf("%c ", map->data[i][j]);
                } else{
                    printf("%c ", ' ');
                }
            }
        }
        printf("\n");
        printf("\e[%d;%dH", ++y, x);
    }
}


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
        p->watchTowerCnt = 10;
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