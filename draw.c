#include "header.h"
#include "types.h"
#include "small_map.c"


const char *charArray[] = {"Attack ", "Run ", "Items ", "Hello "};

void drawBox(int height, int width, int y, int x) {
    printf("\e[%d;%dH", y, x);
    printf("\u250C");
    for(int i=0;i<width;i++) printf("\u2500");
    printf("\u2510\n");
    printf("\e[%d;%dH", ++y, x);

    for(int i=0;i<height;i++){
        printf("\u2502");
        for(int j=0;j<width;j++) printf(" ");
        printf("\u2502\n");
        printf("\e[%d;%dH", ++y, x);
    }

    printf("\u2514");
    for(int i=0;i<width;i++) printf("\u2500");
    printf("\u2518\n");
}

void drawHp(int hp, int hpMax, int y, int x) {
    printf("\e[%d;%dH", y, x);

    printf("|");
    if((float)hp < hpMax*0.3) {
        printf("\e[7;31m");
    } else if((float)hp <= hpMax*0.5) {
        printf("\e[7;33m");
    } else {
        printf("\e[7;32m");
    }

    for(int i=0;i<10;i++){
        if(hp == i) {
            printf("\033[0m");
        }
            printf(" ");
    }

    printf("\033[0m|");
}

void drawChoice(int choice, int y, int x) {
    printf("\e[%d;%dH", y, x);
    for(int i=0; i<3; i++) {
        if(i == choice) {
            printf(ATI "%s\n" RESET, charArray[i]);
        } else {
            printf("%s\n", charArray[i]);
        }
        y++;
        printf("\e[%d;%dH", y, x);
    }
}

void maploop() {
    struct termios old_attr, new_attr;
    tcgetattr(STDIN_FILENO, &old_attr);
    new_attr = old_attr;
    new_attr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);

    int choice = 0;
    char ch;
    while (read(STDIN_FILENO, &ch, 1))
    {
        //delay(0.03);
        printf("\e[1;1H\e[2J");
        printf("\e[?25l");

        if (ch == 's' || ch == 'B') {
            choice = min(choice + 1, 2);
        } else if (ch == 'w' || ch == 'A') {
            choice = max(choice - 1, 0);
        } else if (ch == '\n') {
            break;
        }

        printf("the input char is %c\n", ch);
        printf("the choice is %d\n", choice);
        printHp(6, 10, 7, 3);
        printChoice(choice, 10, 15);
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);
}

int main() {
    Map *map = new_Map(17, 87);
    PlayerData *player = new_PlayerData();
    Game *game = new_Game();
    int smallMapSize = 5;
    game->status = 2;

    gen_maze(map);
    for (int i = 0; i < map->row; i++) {
        for (int j = 0; j < map->col; j++) {
            if(map->data[i][j] == 'P'){
                player->pos = make_IntPair(i, j);
            }
        } 
    }

    printf("\e[1;1H\e[2J");
    printf("\e[?25l");
    drawBox(smallMapSize*2 + 1, 40, 1, 1);
    drawBox(smallMapSize*2 + 1, (smallMapSize*2 + 1)*2 + 1, 1, 43);
    drawMiniMap(map, &player->pos, smallMapSize, 0, 2, 45);
    printf("\e[%d;%dH", 2, 3);
    printf("this is the bug");
    drawChoice(1, 3, 3);
    printf("\n");
    printf("\e[%d;%dH", 17, 1);
}