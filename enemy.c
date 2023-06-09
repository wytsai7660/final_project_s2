#include "header.h"
#include "types.h"

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
