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
#include "FirstWindow.h"
#include "game.h"

trainer ash;

int main(void){
    pokeInfo pokemon[151];
    int startAgain = 0;
    printf("Hola main\n");
    do{
    first(pokemon);
    startAgain = game(pokemon);
    printf("Game Restarting");
    al_rest(2);
    }while(startAgain == 1);
    return 0;
}