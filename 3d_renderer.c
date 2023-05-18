#include "header.h"

bool new_tan(float x, float y, double angle){
    double normAngle = fmod(angle, 2 * M_PI);
    if(normAngle < 0) normAngle += 2 * M_PI; 
    if (normAngle > M_PI/2 && normAngle <= 3*M_PI/2)
    {
        return y >= tan(normAngle)*x;
    }else
    {
        return y <= tan(normAngle)*x;
    }
}

void scan(int row, int col, double pov){
    int degree = (int)M_PI * 400;
    for(int k = 0; k < degree;k++)
    {
        for(int i=-row/2;i<row/2;i++){
            for(int j=-col/2;j<col/2;j++){
                if(new_tan(i, j, k/100.0 - pov/2.0 - M_PI/2) && new_tan(i, j, k/100.0 + pov/2.0 - M_PI/2)){
                    printf("0 ");
                }else{
                    printf("  ");
                }
            }
            printf("\n");
        }
        if (k == 0) usleep(1000*1000);
        usleep(30*1000);
        printf("\e[1;1H\e[2J");

    }

}

int main(){
    scan(30, 50, M_PI/3);
}