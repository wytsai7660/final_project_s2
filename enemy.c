#include "header.h"
#include "types.h"

// event
// boss hp below 20
// boss got damage over 20
// boss hp is 1
// win in the fight
// equal 
// loss

// state
// copy: copy what player's do on previous round
// copy plus: throw what can beat player's last round
// copy min: throw what will loose player's last round
// striaght: keep the same for 2 ~ 5 round
// random: sample from random disturbute
// 

char *moves[] = {"Paper", "Scissor", "Stone"};
char *gameResults[] = {"tie", "enemy win", "you win"};

// boss states
// 0: normal mode
// 1: aggressive mode
// 2: defensive mode
float bossTransferMatrix[2][3][3] = {
{
    // hp below 20
    {0.3, 0.1, 0.6}, // normal mode
    {0.5, 0.3, 0.2}, // aggressive mode
    {0.1, 0.0, 0.9}, // defensive mode
},
{
    // damage over 20
    {0.9, 0.1, 0.0},
    {0.3, 0.3, 0.4},
    {0.1, 0.2, 0.7},
}
};

int roundRemain = 0;
int policy = 0;

// void nextPolicy() {

// }

int enemyNextMove(int lastPlayerMove, int lastEnemyMove) {
    int enemyNextMove = rand_between(0, 2);
    if(!roundRemain) {
        roundRemain = rand_between(2, 5);
        // policy 持續幾回合
        policy = rand_between(0, 4);
        // 選哪個 policy
    }else {
        roundRemain--;
    }
    if(policy == 0) {
        enemyNextMove = lastPlayerMove;
        // 出上次玩家出的
    }
    else if(policy == 1) {
        enemyNextMove = (lastPlayerMove + 1) % 3;
        // 出能贏上次玩家的
    }
    else if(policy == 2) {
        enemyNextMove = (lastPlayerMove + 2) % 3;
        // 出會輸上次玩家的
    }
    else if(policy == 3) {
        enemyNextMove = lastEnemyMove;
        // 出跟上次一樣的
    }
    else if(policy == 4) {
        enemyNextMove = rand_between(0, 2);
        // 隨機
    }

    return enemyNextMove;
}

// real damage = (int) atk * (1 + isCrit ? 2 : 1) / (1 + def / 10)
int solveDamage(PlayerData *p, Enemy *e, Game *g, int playerMove, int enemyMove) {
    int result = (enemyMove - playerMove + 3) % 3;
    if (result == 0) {
        // tie
    } else if (result == 1) {
        // enemy win
        g->isCrit = rand_between(1, 100) <= e->crit;
        g->damage = e->atk * (g->isCrit ? 2 : 1) / (1 + p->def / 10.0);
        p->hp -= (int)g->damage;
    } else if (result == 2) {
        // player win
        g->isCrit = rand_between(1, 100) <= p->crit;
        g->damage = p->atk * (g->isCrit ? 2 : 1) / (1 + e->def / 10.0);
        e->hp -= (int)g->damage;
    }
    return result;
}

void bossPolicy(Game *g, Enemy *e) {
    if(e->hp <= 20) {
        e->bossState = sample(bossTransferMatrix[0][e->bossState], 3);
    }else if(g->damage > 20) {
        e->bossState = sample(bossTransferMatrix[1][e->bossState], 3);
    }
}

void printStates(Enemy *e){
    printf("n_states: %d\n", e->n_states);

    for(int i=0;i<e->n_states;i++){
        for(int j=0;j<e->n_states;j++){

            printf("%.2f ", e->data[i][j]);

        }
        printf("\n");
    }
}
