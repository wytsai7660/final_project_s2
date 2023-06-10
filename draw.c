#include "header.h"
#include "types.h"


const char *charArray[] = {"Attack ", "Run ", "Items ", "Hello "};

void drawBox(int height, int width, int y, int x) {
    printf("\e[%d;%dH", y, x);
    printf("\u250C");
    for(int i=0;i<width-2;i++) printf("\u2500");
    printf("\u2510\n");
    printf("\e[%d;%dH", ++y, x);

    for(int i=0;i<height-2;i++){
        printf("\u2502");
        for(int j=0;j<width-2;j++) printf(" ");
        printf("\u2502\n");
        printf("\e[%d;%dH", ++y, x);
    }

    printf("\u2514");
    for(int i=0;i<width-2;i++) printf("\u2500");
    printf("\u2518\n");
}

// coordinate on center
void drawSolidBox(int height, int width, int tick, int y, int x) {
    for(int i=0;i<height;i++) {
      printf("\e[%d;%dH", y + i, x - width / 2);
      for(int j=0;j<width;j++) {
        printf("%c", j%2 + tick%2 ? ' ' : '@');
      }
      printf("\n");
    }
}

void drawHp(int hp, int hpMax) {

    printf("HP: |");
    if((float)hp < hpMax*0.3) {
        printf("\e[7;31m");
    } else if((float)hp <= hpMax*0.5) {
        printf("\e[7;33m");
    } else {
        printf("\e[7;32m");
    }

    for(int i=0;i<hpMax;i++){
        if(hp == i) {
            printf("\033[0m");
        }
            printf(" ");
    }

    printf("\033[0m|\n");
}

void drawChoice(char *option[], int choice, int y, int x) {
    printf("\e[%d;%dH", y, x);
    for(int i=0; i<3; i++) {
        if (i == choice) printf(ALT);
        printf("%s", option[i]);
        printf(RESET  "   ");
    }
    printf("\n");
}

void drawStatusBar(PlayerData *p, int panelWidth, int y, int x) {
    printf("\e[%d;%dH", y, x);
    printf("LIFE: ");
    printf(RED);

    for(int i=0;i<5;i++){
        if(p->life > i) {
            printf("❤  ");
        } else {
            printf("    ");
        }    
    }

    printf(RESET);
    // 26
    // for(int i=0;i<6+4*5)

    printf("       | ATK: %-10d | DEF: %-10d | CRIT: %d%%         | ", p->atk, p->def, p->crit);
    
    drawHp(p->hpMax, p->hpMax);
}

void drawBackpack(PlayerData *p, Game *g, int choice, int y, int x) {
    drawBox(8, 70, y, x);
    printf("\e[%d;%dH", y + 1, x + 2);
    printf("BACKPACK");
    printf("\e[%d;%dH  ", y + 3, x + 2);
    
    for(int i=0; i<4; i++) {
        if(p->backpack[i] == 0 || !items_maze_usability[i]) {
            printf(GRY);
        } else {
            if(g->items_enabled[i]) printf(YEL);
        }
        if(i == choice) printf(ALT);
        printf("%s", items_name[i]);
        printf(RESET);
        printf("   ");
    }

    printf("\e[%d;%dH  ", y + 4, x + 2);
    printf("    %-17d%-18d%-15d%d", p->backpack[0], p->backpack[1], p->backpack[2], p->backpack[3]);

    printf("\e[%d;%dH", y + 2, x + 2);
    printf("\e[%d;%dH", y + 6, x + 2);
    printf("     [A][D] To Choose item  [ENTER] To Activate  [Q] To Leave");
}


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
