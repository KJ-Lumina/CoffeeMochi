#include "cprocessing.h"
#include <stdio.h>
#include <stdlib.h>
//#include <Resource_stats.h>


#define COMMON 0 
#define SECOND 1
#define FIRST 2

#define STUDENT 0
#define ELDERLY 1
#define KID 2

#define OFFICEWORKER 0 
#define RETAILOWNER 1
#define TEACHER 2

#define DOCTOR 0
#define MINISTER 1
#define LAWYER 2


#define SAD 0 //morale is low 
#define HUNGRY 1 //low on food
#define BROKE 2 //no money 
#define HAPPY 3 //too high on morale
#define FULL 4 // too much food
#define RICH 5 // too much gold

int tier




struct PROFILE{
    char name[];
    //char descrip[];
    int age;
    int needs;
    char occupation[];

};

PROFILE create_citizen(){
int second_gen;
int first_gen;
int tier_gen= (rand() % 20);
    if (tier_gen >= 0 && tier_gen< 10) {
        tier = COMMON;
        int common_gen = (rand() % 3);
        int name_gen = (rand() % 5);
        
        switch (common_gen) {
            
        case STUDENT: //student
            //{ "student", "elderly", "kid" };
            occupation = "student";
            age = (rand() % 10) + 13;
            if (name_gen == 0) {
                name ="student john";
            }
            else if (name_gen == 1) {
                name ="student karen";
            }
            else if (name_gen == 2) {
                name ="student bob";
            }
            else if (name_gen == 3) {
                name ="student nancy";
            }
            else if (name_gen == 4) {
                name ="student george";
            }
            else if (name_gen == 5) {
                name ="student travess";
            }

            break;

        case ELDERLY:
            occupation = "senior citizen";
            age = (rand() % 10) + 60;
            int name_gen = (rand() % 5);

            if (name_gen == 0) {
                name = "elderly john";
            }
            else if (name_gen == 1) {
                name = "elderly karen";
            }
            else if (name_gen == 2) {
                name = "elderly bob";
            }
            else if (name_gen == 3) {
                name = "elderly nancy";
            }
            else if (name_gen == 4) {
                name = "elderly george";
            }
            else if (name_gen == 5) {
                name = "elderly travess";
            }
            break;

        case KID:
            occupation = "kid";
            age = (rand() % 8) +5;
            if (name_gen == 0) {
                name = "kid john";
            }
            else if (name_gen == 1) {
                name = "kid karen";
            }
            else if (name_gen == 2) {
                name = "kid bob";
            }
            else if (name_gen == 3) {
                name = "kid nancy";
            }
            else if (name_gen == 4) {
                name = "kid george";
            }
            else if (name_gen == 5) {
                name = "kid travess";
            }

            break;

        }
        
    }
    else if (tier_gen >= 10 && tier_gen < 17) {
        tier = SECOND;
        int name_gen = (rand() % 5);
        second_gen = rand() % 3;
        switch (second_gen) {
        case OFFICEWORKER://0
            occupation = "office worker";
            age = (rand() % 8) + 30;
            if (name_gen == 0) {
                name = "colleague john";
            }
            else if (name_gen == 1) {
                name = "colleague karen";
            }
            else if (name_gen == 2) {
                name = "colleague bob";
            }
            else if (name_gen == 3) {
                name = "colleague nancy";
            }
            else if (name_gen == 4) {
                name = "colleague george";
            }
            else if (name_gen == 5) {
                name = "colleague travess";
            }
            break;


        case RETAILOWNER://1
            age = (rand() % 8) + 25;
            occupation = "retail owner";
            int name_gen = (rand() % 5);
            if (name_gen == 0) {
                name = "boss john";
            }
            else if (name_gen == 1) {
                name = "boss karen";
            }
            else if (name_gen == 2) {
                name = "boss bob";
            }
            else if (name_gen == 3) {
                name = "boss nancy";
            }
            else if (name_gen == 4) {
                name = "boss george";
            }
            else if (name_gen == 5) {
                name = "boss travess";
            }
            break;

        case TEACHER://2
            occupation = "teacher";
            age = (rand() % 8) + 20;
            if (name_gen == 0) {
                name = "teacher john";
            }
            else if (name_gen == 1) {
                name = "teacher karen";
            }
            else if (name_gen == 2) {
                name = "teacher bob";
            }
            else if (name_gen == 3) {
                name = "teacher nancy";
            }
            else if (name_gen == 4) {
                name = "teacher george";
            }
            else if (name_gen == 5) {
                name = "teacher travess";
            }
            break;

        }
    }
    else {
        tier = FIRST;
        occupation = "doctor";
        first_gen = rand() % 3;
        switch (second_gen) {
        case DOCTOR://0
            age = (rand() % 8) + 20;
            if (name_gen == 0) {
                name = "teacher john";
            }
            else if (name_gen == 1) {
                name = "teacher karen";
            }
            else if (name_gen == 2) {
                name = "teacher bob";
            }
            else if (name_gen == 3) {
                name = "teacher nancy";
            }
            else if (name_gen == 4) {
                name = "teacher george";
            }
            else if (name_gen == 5) {
                name = "teacher travess";
            }
            break;


        case MINISTER://1
            occupation = "minister";
            age = (rand() % 8) + 20;
            if (name_gen == 0) {
                name = "teacher john";
            }
            else if (name_gen == 1) {
                name = "teacher karen";
            }
            else if (name_gen == 2) {
                name = "teacher bob";
            }
            else if (name_gen == 3) {
                name = "teacher nancy";
            }
            else if (name_gen == 4) {
                name = "teacher george";
            }
            else if (name_gen == 5) {
                name = "teacher travess";
            }
            break;


        case LAWYER://2
            occupation = "lawyer";
            age = (rand() % 8) + 20;
            if (name_gen == 0) {
                name = "lawyer john";
            }
            else if (name_gen == 1) {
                name = "lawyer karen";
            }
            else if (name_gen == 2) {
                name = "lawyer bob";
            }
            else if (name_gen == 3) {
                name = "lawyer nancy";
            }
            else if (name_gen == 4) {
                name = "lawyer george";
            }
            else if (name_gen == 5) {
                name = "lawyer travess";
            }
            break;

        }
    }
    
    return PROFILE{ name, age , occupation };
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
