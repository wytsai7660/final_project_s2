#include "map.c"
#include "header.h"
#include <termios.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define ATI   "\x1B[17m"
#define RESET "\x1B[0m"
#define DEBUG


void drawMiniMap(Map *map, IntPair *playerPos, int smallMapSize, int watchTowerCnt){
    printf("player pos %d, %d\n", playerPos->first, playerPos->second);

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
    }
}

void fight() {
    printf("fight\n");
}

void gainItem() {
    printf("gainItem\n");
}

void playerEvent(Map *m, IntPair *playerPos, PlayerData *p){
    char ch = m->data[playerPos->first][playerPos->second];
    m->data[playerPos->first][playerPos->second] = '9';
    switch (ch)
    {
    case '0':
        p->hp++;
        break;
    case '1':
        p->hp = max(p->hp-1, 0);
        break;
    case '2':
        p->atk++;
        break;
    case '3':
        p->atk = max(p->atk-1, 1);
        break;
    case '4':
        p->def++;
        break;
    case '5':
        p->def = max(p->def-1, 0);
        break;
    case '6':
        fight();
        break;
    case '7':
        p->watchTowerCnt = 10;
        break;
    case '8':
        gainItem();
        break;
    default:
        break;
    }
}


int main(){
    struct termios old_attr, new_attr;
    tcgetattr(STDIN_FILENO, &old_attr);
    new_attr = old_attr;
    new_attr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);


    srand((unsigned)time(NULL));
    Map *map = new_Map(17, 87);
    PlayerData *player = new_PlayerData();
    int smallMapSize = 5;
    IntPair playerPos;
    char ch;

    gen_maze(map);
    for (int i = 0; i < map->row; i++) {
        for (int j = 0; j < map->col; j++) {
            if(map->data[i][j] == 'P'){
                playerPos = make_IntPair(i, j);
            }
        } 
    }
    int dir;

    // game loop
    while(read(STDIN_FILENO, &ch, 1) != '0')
    {
        printf("\e[1;1H\e[2J");
        printf("\e[?25l");
        printf("hp %d atk %d def %d\n", player->hp, player->atk, player->def);
        printf("watchTowerCnt %d\n", player->watchTowerCnt);
        drawMiniMap(map, &playerPos, smallMapSize, player->watchTowerCnt);
        if(ch == 'w')
        {
            dir = 2;
        }
        else if(ch == 'a')
        {
            dir = 1;
        }
        else if(ch == 's')
        {
            dir = 0;
        }
        else if(ch == 'd')
        {
            dir = 3;
        }else{
            continue;
        }
        
        // clear affect
        player->watchTowerCnt -= player->watchTowerCnt ? 1 : 0;

        if (!(map->data[playerPos.first + direction[dir][0]][playerPos.second + direction[dir][1]] == '@'))
        {
            playerPos.first += direction[dir][0];
            playerPos.second += direction[dir][1];
            playerEvent(map, &playerPos, player);
        }
        usleep(30*1000);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);
}