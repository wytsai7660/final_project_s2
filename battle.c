#include "header.h"
#include "enemy.c"

const char *moves[] = {"Paper", "Scissor", "Stone"};

int main() {
    PlayerData *p = new_PlayerData();
    Enemy *e = new_Enemy(p, 3);
    char ch;
    printf("your are fighting with the monster, choose how to attack\n");
    while (p->hp > 0 && e->hp > 0) {
        printf("\nmonster's hp: %d atk: %d def: %d\n", e->hp, e->atk, e->def);
        printf("your hp: %d atk: %d def: %d\n", p->hp, p->atk, p->def);
        printf("[Q] %s\n", moves[0]);
        printf("[W] %s\n", moves[1]);
        printf("[E] %s\n", moves[2]);
        ch = getchar();
        getchar();
        
        bool isCrit = false;
        int damage, result;
        int enemyMove = sample(e->moveDistrube, 3);
        printf("enemyMove: %d\n", enemyMove);

        // if win -> you attack
        // if lose -> enemy attack
        // if tie -> nothing happened
        if(ch == 'q' && enemyMove == 0 || ch == 'w' && enemyMove == 1 || ch == 'e' && enemyMove == 2){
            result = 0;
            printf("Tie!!\n");
        }else if(ch == 'q' && enemyMove == 1 || ch == 'w' && enemyMove == 2 || ch == 'e' && enemyMove == 0) {
            isCrit = rand_between(1, 100) <= e->crit;
            damage = e->atk * (isCrit ? 2 : 1) / (float)(1 + p->def/100);
            p->hp -= damage;
            result = -1;
            printf("Monster attack!! damage: %d\n", damage);
        }else if(ch == 'q' && enemyMove == 2 || ch == 'w' && enemyMove == 0 || ch == 'e' && enemyMove == 1) {
            isCrit = rand_between(1, 100) <= p->crit;
            damage = p->atk * (isCrit ? 2 : 1) / (float)(1 + e->def/100);
            e->hp -= damage;
            result = 1;
            printf("Player attack!! damage: %d\n", damage);
        }else {
            printf("invalid input!\n");
        }
        

    }
    if(p->hp <= 0){
        p->life--;
        printf("player loose!\n");
    }else{
        printf("monster defeated!\n");
    }
}
    
