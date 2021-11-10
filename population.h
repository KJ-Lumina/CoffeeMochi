#include "cprocessing.h"
#include <stdio.h>
#include <stdlib.h>
//#include <Resource_stats.h>


#define STUDENT 0
#define ELDERLY 1
#define KID 2

int i;
int c;
int s;
int a;
int g;
int current_morale = 50;
int current_food = 50;
int current_gold = 50;


//struct citizen {
//
//    int morale;
//    int food;
//    int gold;
//
//};
//struct common_citizen {
//
//};
//struct second_tier {
//
//};
//struct first_tier {
//
//};

//void common_citizen_s() {
//    //https://overiq.com/c-programming-101/array-of-strings-in-c/
//
//}
void common_citizen() {
    c = (rand() % 4);
    g = (rand() % 2);
    //char ans, Y='y', N='n';
    char common[4][30] = { "student", "elderly", "kid", "cleaner" };

    if (c == STUDENT) {
        switch (g)
        {
        case 0:
            //crops decrease by 5
            current_food = current_food - 5;
            //morale increase by 5
            current_morale = current_morale + 5;
            //finance increase by 5
            current_gold = current_gold + 5;
            printf("I am an immigrant and would like to stay in your country to study\n");
            printf("%d,%d,%d", current_food, current_gold, current_morale);
            break;

        case 1:
            //crops decrease by 5
            current_food = current_food - 5;
            //morale increase by 10
            current_morale = current_morale + 10;
            //finance decrease by 5
            current_gold = current_gold - 5;

            printf("I need an education and i come on a scholarship\n");
            break;
        }

    }
    else if (c == ELDERLY) {
        switch (g)
        {
        case 0:
            //crops decrease by 3
            current_food = current_food - 5;
            //morale increase by 5
            current_morale = current_morale + 10;
            //finance supported

            printf("I am seeking shelter from the storm \n");
            break;

        case 1:

            printf("my family is inside please let me in\n");
            //if ((scanf_s("y or n:%c\n", ans))==Y) {
                //crops decrease by 5
            current_food = current_food - 5;
            //morale increase by 10
            current_morale = current_morale + 10;
            //finance decrease by 5
            current_gold = current_gold - 5;
            printf("changes been made");
            //}
            break;
        }
    }
    printf("%s", common[c]);
    //common citizen common stats
    //farmer boost crops boost money
    //chef boost happiness reduce crops increase money 
    // use random int to cycle between characters if create citizen lands on common citizen 

}
void special_citizen() {
    s = (rand() % 4);
    char special[4][60] = { "doctor", "chef", "farmer", "banker" };
    //citizens like doctor etc
    //use random int to cycle between characters if create citizen lands on special citizen 
    //maybe put array in array 

    printf("%s", special[s]);
}
void authorized_citizen() {
    a = (rand() % 4);
    char authorized[4][60] = { "advisor", "soldier", "governor", "detective" };
    printf("%s", authorized[a]);
    //lawyer, army, detective
    //use random int to cycle between characters if create citizen lands on authorized citizen
}
void create_citizen() {
    //3 kinds citizen array
    //55% for common citizen
    //30% for second tier
    //15% for first tier
    int ir;
    ir = (rand() % 20);
    if (ir >= 0 && ir <= 10) {
        i = 0;
    }
    else if (ir > 10 && ir <= 16) {
        i = 1;
    }
    else if (ir > 16 && ir < 20) {
        i = 2;
    }
    const char citizen[3][20] = { "civilian", "second tier", "first tier" };
    //--------------------------------------------------------------------
    printf("%s\n", &citizen[i]);

    //switch case for types of citizen
    if (i == 0) {
        common_citizen();
    }
    else if (i == 1) {
        special_citizen();
    }
    else {
        authorized_citizen();
    }
    //-------------------------------------------------------------------
    //set into new variables into array 

}
void citizen() {
    //calls create citizen, runs and display chosen citizen 
}
void game_init(void)
{

}
void game_update()
{
    //TODO: Your implementations here
    //3 kinds of citizen 
    //use random int
    // cycle between 3 citizens
    int number;
    scanf_s("%d", &number);
    if (number == 3) {
        create_citizen();
    }
}


void game_exit(void)
{

}
