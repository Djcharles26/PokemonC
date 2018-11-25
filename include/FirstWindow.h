#ifndef FIRST_H_
#define FIRST_H_
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/timer.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "allegrolib.h"

#define PokePath "./resources/generation-1/red-blue/"

typedef struct{
    char name[100];
    int damage;
    int accuracy;
}attack;

typedef struct{
    char name[100];
    char type[100];
    int evolve;//# of pokemon it evolves to
    bool isRoot;
    bool isChoosed;
    int ps;//
    int at;//attack
    int def;//defense
    int vel;//velocity
    int ac;//accuracyint lvl;//level
    int hp;//Start always with 100
    int lvl;
    int lvlToEvolve;
    attack ataques[4];
    al_image image;
    int index;
}pokeInfo;

typedef struct{
    char name[100];
    int ch;
    int medallas;
    pokeInfo p[3];
    int pokeQ;
    int lvl;
}trainer;

typedef struct{
    int x;
    int y;
}coords;

extern int first(pokeInfo pokemon[151]);

#endif