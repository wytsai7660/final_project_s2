#include "header.h"


bool new_tan(float x, float y, double angle){
    double normAngle = fmod(angle, 2 * PI);
    if(normAngle < 0) normAngle += 2 * PI; 
    if (normAngle > PI/2 && normAngle <= 3*PI/2)
    {
        return y >= tan(normAngle)*x;
    }else
    {
        return y <= tan(normAngle)*x;
    }
}

void scan(int row, int col, double pov){
    int degree = (int)PI * 400;
    for(int k = 0; k < degree;k++)
    {
        for(int i=-row/2;i<row/2;i++){
            for(int j=-col/2;j<col/2;j++){
                if(new_tan(i, j, k/100.0 - pov - PI/2) && new_tan(i, j, k/100.0 - PI/2)){
                    printf("0 ");
                }else{
                    printf("  ");
                }
            }
            printf("\n");
        }
        if (k == 0) usleep(5000*1000);
        usleep(30*1000);
        printf("\e[1;1H\e[2J");

    }

}

void printFOV(int row, int col, double pov){

    for(int i=-row/2;i<row/2+1;i++){
        for(int j=-col/2;j<col/2+1;j++){
            if(new_tan(i, j,  - pov) && new_tan(i, j,  0)){
                printf("0 ");
            }else{
                printf("  ");
            }
        }
        printf("\n");
    }
    usleep(1000*1000);
}

int main(){
    // scan(30, 50, fov);
    printFOV(30, 50, PI/6);
    printFOV(30, 50, PI/5);
    printFOV(30, 50, PI/4);
    printFOV(30, 50, PI/3);
    printFOV(30, 50, PI/2);
    printFOV(30, 50, 2*PI/3);
}