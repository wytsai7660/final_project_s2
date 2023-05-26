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
#define RESET "\x1B[0m"


void drawSmallMap(Map *map, IntPair *playerPos, int smallMapSize){
    printf("player pos %d, %d\n", playerPos->first, playerPos->second);

    int xcenter = max(smallMapSize, min(playerPos->first, map->row - 1 - smallMapSize));
    int ycenter = max(smallMapSize, min(playerPos->second, map->col - 1 - smallMapSize));

    for(int i = xcenter - smallMapSize; i <= xcenter + smallMapSize; i++){
        for(int j = ycenter - smallMapSize; j <= ycenter + smallMapSize; j++){
            if(playerPos->first == i && playerPos->second == j){
                printf(RED "%c " RESET, 'P');
            }else if(map->data[i][j] == '@'){
                printf("%c ", '@');
            }else if(map->data[i][j] == '9'){
                printf("%c ", ' ');
            }else{
                printf("%c ", map->data[i][j]);
            }
        }
        printf("\n");
    }
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
        drawSmallMap(map, &playerPos, smallMapSize);
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

        if (!(map->data[playerPos.first + direction[dir][0]][playerPos.second + direction[dir][1]] == '@'))
        {
            playerPos.first += direction[dir][0];
            playerPos.second += direction[dir][1];
        }
        playerEvent(map, &playerPos, player);
        usleep(30*1000);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);
}