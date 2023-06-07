#include "header.h"
#include "types.h"
#include <termios.h>


const char *charArray[] = {" Attack ", " Run ", " Items ", " Hello "};

void printHp(int hp, int hpMax, int y, int x) {
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

void printChoice(int choice, int y, int x) {
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

int main() {
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