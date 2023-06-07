#include <stdio.h>

int main() {
    printf("\u250C");
    for(int i=0;i<5;i++){
        printf("\u2500");
    }
    printf("\u2510\n");
    for(int i=0;i<4;i++){
        printf("\u2502");
        for(int j=0;j<5;j++){
            printf(" ");
        }
        printf("\u2502\n");
    }
    printf("\u2514");
    for(int i=0;i<5;i++){
        printf("\u2500");
    }
    printf("\u2518\n");
    
    return 0;
}
