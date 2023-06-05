#include "header.h"
#include "types.h"

typedef struct {
    float **data;
    int n_states;
    int atk;
    int def;
    int hp;
    int crit;
    float *moveDistrube;
}Enemy;

Enemy *new_Enemy(PlayerData *p, int n_states){
    if(n_states < 2) return NULL;
    Enemy *e = malloc(sizeof(Enemy));

    float sum = 0;
    float *moveDistrube = malloc(sizeof(float) * 3);
    for(int i = 0;i < 3;i++){
        moveDistrube[i] = (float)rand() / RAND_MAX;
        sum += moveDistrube[i];
    }

    // normalize the numbers to sum up to 1
    for(int i = 0;i < 3;i++){
        moveDistrube[i] /= sum;
    }

    float **tmp = malloc(n_states * sizeof(float *));
    for (int i = 0; i < n_states; i++) tmp[i] = memset(malloc(n_states * sizeof(float)), 0, n_states);
    *e = (Enemy){tmp, n_states, rand_between(p->hp/10 + 1, p->hp/3 + 1), rand_between(p->atk/10 + 1, p->atk/2 + 1), rand_between(5, 10), rand_between(0, 10), moveDistrube};
    return e;
}

// event
// boss hp below 20
// boss got damage over 20
// boss hp is 1
// win in the fight
// equal 
// loss


// player
// real hit = atk * (1 + crit)
// real damage = (int) damage / (10 + def)

void printStates(Enemy *e){
    printf("n_states: %d\n", e->n_states);

    for(int i=0;i<e->n_states;i++){
        for(int j=0;j<e->n_states;j++){

            printf("%.2f ", e->data[i][j]);

        }
        printf("\n");
    }
}

void nextState(Enemy *e){

}


// int main(){
//     Enemy *e = new_Enemy(5);
//     printStates(e);
// }