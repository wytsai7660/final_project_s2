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

const char *moves[] = {"Paper", "Scissor", "Stone"};
int roundRemain = 0;
int policy = 0;
int policyWinRate[5][2] = {0};

int resolve(int lastPlayerMove, int lastEnemyMove) {
    int enemyNextMove = rand_between(0, 2);
    if(!roundRemain) {
        roundRemain = rand_between(2, 5);
        // policy 持續幾回合
        policy = rand_between(0, 4);
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
    }

    return enemyNextMove;
}

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

int main() {
    srand(time(NULL));
    PlayerData *p = new_PlayerData();
    Enemy *e = new_Enemy(p, 5);
    e->PlayerMove = rand_between(0, 2);
    e->EnemyMove = rand_between(0, 2);
    int playerwin = 0, enemywin = 0;
    int move;
    for(int i=0;i<10000;i++)
    {
        printf("round remain %d, policy %d\n", roundRemain, policy);
        printf("your move: ");
        //scanf("%d", &move);
        move = sample(e->moveDistrube, 3);
        e->EnemyMove = resolve(e->PlayerMove, e->EnemyMove);
        e->PlayerMove = move;
        printf("enemys move: %d\n", e->EnemyMove);
        switch ((e->EnemyMove - e->PlayerMove + 3) % 3)
        {
        case 1:
            printf("enemy win\n");
            policyWinRate[policy][0]++;
            enemywin++;
            break;
        case 2:
            printf("player win\n");
            policyWinRate[policy][1]++;
            playerwin++;
            break;
        case 0:
            printf("tie\n");
            break;
        
        default:
            break;
        }
    }
    printf("enemy win %d, player win %d\n", enemywin, playerwin);
    for(int i=0;i<5;i++) {
        printf("policy %d enemy win %d, player win %d\n", i, policyWinRate[i][0], policyWinRate[i][1]);
    }
}