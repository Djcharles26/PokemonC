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

extern trainer ash;

int first(pokeInfo pokemon[151]){
    printf("Hola window");
    srand(time(NULL));
    ALLEGRO_DISPLAY *window;
    ALLEGRO_CONFIG *pokedex = al_load_config_file("./resources/pokemon.cfg");
    if(!pokedex){
        fprintf(stderr,"It couldnt charge all the pokemon data");
    }
    allegro_start(&window);
    al_rest(1);
    int i,cnt=0;
    int pokemonGridPicker[9][16];
    for(i = 0; i<9; i++){
        for(int j=0; j<16; j++){
            pokemonGridPicker[i][j]=-1;
        }
        printf("%d",i);
        fflush(stdout);
    }
    int colorNumber = 0, selected = -1;
    coords pokSelected;
    pokSelected.x = 1;
    pokSelected.y = 1;
    float frameTime = 0,deltaTime = 1.0/60, delay = 0;
    bool running = true, isBlack = true,enter= false,draw = false, itChanged = false;
    bool chSelecting = false,naming = false;
    attack commonAttacks[4];
    strcpy(commonAttacks[0].name,"Tackle");
    commonAttacks[0].damage = 20;
    commonAttacks[0].accuracy = 40;
    strcpy(commonAttacks[1].name,"Tail Whip");
    commonAttacks[1].damage = 30;
    commonAttacks[1].accuracy = 40;
    strcpy(commonAttacks[2].name,"Growl");
    commonAttacks[2].damage = 10;
    commonAttacks[2].accuracy = 20;
    strcpy(commonAttacks[3].name,"Ember");
    commonAttacks[3].damage = 50;
    commonAttacks[3].accuracy = 40;
    //trainer ash;
    memset(&ash,0,sizeof(trainer));
    fflush(stdout);
    printf("I memseted ash\n");
    ash.lvl = 1;
    ash.pokeQ = 3;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;//QUEUE EVENT
    ALLEGRO_FONT  *shade = NULL, *roboto1 =NULL, *NAME = NULL;//FONTS
    ALLEGRO_USTR *input = al_ustr_new("");//Text input
    al_Rect SelectionRect,SelectionRect2;
    SelectionRect.x = 140;
    SelectionRect.y = 120;
    SelectionRect.w = 180;
    SelectionRect.h = 280;
    SelectionRect2.x = 120;
    SelectionRect2.y = 130;
    al_image start,background,character,pok[151],menu;
    //pokeInfo pokemon[151];
    //Texture creations
    fflush(stdout);
    printf("Oola");
    start = al_load_image("./resources/Stadiumstart.jpg");
    fflush(stdout);
    printf("Hola created\n");
    background = al_load_image("./resources/psyBack.jpg");
    fflush(stdout);
    printf("Hola psyBack\n");
    character = al_load_image("./resources/trainer_sprites.png");
    fflush(stdout);
    printf("Hola trainer_sprites\n");
    char buffer[10];
    char aux[10];
    char dest[100];
    char *ptr;
    for(int i = 1; i<152; i++){
        memset(dest,0,sizeof(dest));
        fflush(stdin);
        sprintf(buffer,"%d.png",i);
        sprintf(aux,"%d",i);
        //printf("%s\n",buffer);
        strcpy(dest,PokePath);
        strcat(dest,buffer);
        pok[i-1] = al_load_image(dest);
        pokemon[i-1].image = pok[i-1];
        strcpy(pokemon[i-1].name,al_get_config_value(pokedex,aux,"name"));
        strcpy(pokemon[i-1].type,al_get_config_value(pokedex,aux,"type"));
        pokemon[i-1].evolve = strtod(al_get_config_value(pokedex,aux,"evolve"),&ptr);
        pokemon[i-1].isRoot = strtod(al_get_config_value(pokedex,aux,"isRoot"),&ptr);
        pokemon[i-1].ps = strtod(al_get_config_value(pokedex,aux,"ps"),&ptr);
         pokemon[i-1].at = strtod(al_get_config_value(pokedex,aux,"at"),&ptr);
        pokemon[i-1].def = strtod(al_get_config_value(pokedex,aux,"def"),&ptr);
        pokemon[i-1].vel = strtod(al_get_config_value(pokedex,aux,"vel"),&ptr);
        pokemon[i-1].ac = strtod(al_get_config_value(pokedex,aux,"ac"),&ptr);
        pokemon[i-1].index = i;
        pokemon[i-1].hp = 100;
        pokemon[i-1].lvl = 1;
        pokemon[i-1].isChoosed = false;
        pokemon[i-1].lvlToEvolve = pokemon[i-1].lvl + 20 + rand()%10;
        for(int j = 0; j<4; j++){
            pokemon[i-1].ataques[j] = commonAttacks[j];
        }
    }


    if(!(event_queue = al_create_event_queue()))return -1;
    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_TIMER *timer = al_create_timer(1.0/60);
    ALLEGRO_EVENT ev;
    ALLEGRO_COLOR redRect  = al_map_rgb(255,12,14), white = al_map_rgb(255,255,255);
    ALLEGRO_COLOR black = al_map_rgba(0,0,0,255), blackSteady = al_map_rgb(0,0,0);
    register_event_sources(window,timer,&event_queue);
    if(!(roboto1=  al_load_ttf_font("./resources/Fonts/Early-GameBoy.ttf",23,0)))
        fprintf(stderr,"ERROR CREATING FONT\n");

    if(!(shade = al_load_ttf_font("./resources/Fonts/Early-GameBoy.ttf",24,0)))
        fprintf(stderr,"ERROR CREATING FONT \n");

    if(!(NAME = al_load_ttf_font("./resources/Fonts/Early-GameBoy.ttf",100,0)))
        fprintf(stderr,"ERROR CREATING FONT\n");

    
    
    al_start_timer(timer);
    while(running){
        al_get_keyboard_state(&keyState);
        al_wait_for_event(event_queue,&ev);
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            printf("I ended");
            running = false;
        }else if(al_key_down(&keyState,ALLEGRO_KEY_ESCAPE)){
                running = false;
        }else if(al_key_down(&keyState,ALLEGRO_KEY_ENTER) && !enter){
                printf("You pressed enter\n");
                enter = true;
                al_rest(0.5);
        }else if(ev.type == ALLEGRO_EVENT_TIMER){
            frameTime += deltaTime;
            if(frameTime >= 1){
                frameTime = 0;
                if(isBlack){
                    isBlack = false;
                    colorNumber = 255;
                    black.a = 00;
                }else{
                    isBlack = true;
                    colorNumber = 0;
                    black.a = 255;
                }
            }
            draw = true;
        }else if(al_key_down(&keyState,ALLEGRO_KEY_RIGHT) && !itChanged && SelectionRect.x<=702){
            SelectionRect.x += 278.5;
            itChanged = true;
            
        }else if(al_key_down(&keyState,ALLEGRO_KEY_LEFT) && !itChanged && SelectionRect.x>=417){
            SelectionRect.x -= 278.5;
            itChanged = true;
        }else if(al_key_down(&keyState,ALLEGRO_KEY_ENTER) && !chSelecting){
            int aux = SelectionRect.x;
            chSelecting = true;
            naming = true;
            printf("%d",aux);
            switch(aux){
                case 140:
                    printf("You choosed ASH\n");
                    ash.ch=0;
                break;
                case 418:
                    printf("You choosed LOUIS\n");
                    ash.ch = 1;
                break;
                case 697:
                    printf("You choosed ANA\n");
                    ash.ch = 2;
                break;
                case 975:
                    printf("You choosed DANIEL\n");  
                    ash.ch = 3;
                break;
            }
            al_rest(.3);
        }
        if(!al_key_down(&keyState,ALLEGRO_KEY_RIGHT) &&
            !al_key_down(&keyState,ALLEGRO_KEY_LEFT)){
            itChanged = false;
        }


        if(naming){
            if(ev.type == ALLEGRO_EVENT_KEY_CHAR && strlen(ash.name)<5){
                int unichar = ev.keyboard.unichar;
                if(unichar>=32){
                    al_ustr_append_chr(input,unichar);
                    strcpy(ash.name, al_cstr(input));
                } 
            }else if(al_key_down(&keyState,ALLEGRO_KEY_BACKSPACE) && strlen(ash.name)>=1){
                ash.name[strlen(ash.name)-1]='\0';
                al_ustr_free(input);
                input = al_ustr_new(ash.name);
                al_rest(.2);
            }
        }

        //DRAW ZONE:
        if(draw && al_event_queue_is_empty(event_queue)){
            //START
            if(!enter){
                al_draw_bitmap(start.bitmap,13,1,0);
                al_draw_text(shade,al_map_rgba(colorNumber,colorNumber,colorNumber,255),
                    SCREEN_WIDTH/2-190, SCREEN_HEIGHT/2 +200,0,"PRESS ENTER TO START");
            }else if(enter && !chSelecting){
                al_clear_to_color(al_map_rgba(0,0,0,255));
                al_draw_scaled(background,0,0);
                al_draw_text(shade,white,480,40,0,"CHOOSE A CHARACTER");
                al_draw_character(character,0,4,0,false);
                al_draw_rectangle(SelectionRect.x,SelectionRect.y,
                    SelectionRect.x + SelectionRect.w,SelectionRect.y + SelectionRect.h,
                    redRect,12);
            }else if(enter && chSelecting && naming){
                al_clear_to_color(al_map_rgb(0,0,0));
                al_draw_scaled(background,0,0);
                al_draw_text(shade,white,SCREEN_WIDTH/2-100,40,0,"GIVE A NAME");
                for(int i=1; i<6; i++){
                    al_draw_line((SCREEN_WIDTH/2)-230 +(100 * i),SCREEN_HEIGHT/2,(SCREEN_WIDTH/2)-300 + (100 * i),
                        SCREEN_HEIGHT/2,black,8);
                    
                }
                if(al_key_down(&keyState, ALLEGRO_KEY_ENTER) && strlen(ash.name)>=1){
                    printf("Enter pressed \n");
                    naming = false;
                    for(int i=0; i<strlen(ash.name); i++){
                        ash.name[i]=putchar(toupper(ash.name[i]));
                    }
                    al_rest(.3);
                }
                al_draw_justified_text(NAME,blackSteady,(SCREEN_WIDTH/2) -200, SCREEN_WIDTH/2 + 20,
                SCREEN_HEIGHT/2-160,100,0,ash.name);
                al_draw_text(shade,white,SCREEN_WIDTH/2-150,680,0,"ENTER WHEN FINISH");
            }else if(enter && chSelecting && !naming && cnt<3){
                al_clear_to_color(al_map_rgb(0,0,0));
                al_draw_scaled(background,0,0); 
                al_draw_text(shade,blackSteady,SCREEN_WIDTH/2-40,40,0,ash.name);
                al_draw_text(shade,blackSteady,SCREEN_WIDTH/2-150,70,0,"CHOOSE 3 POKEMON");
                int x=0,y=0,auxX=0,gray=0;
                for(int i=0; i<151; i++){
                    if(pokemon[i].isRoot){
                        pokemonGridPicker[y][x]=i;
                        if(pokemon[i].isChoosed){
                            gray = 100;
                        }else{
                            gray = 255;
                        }
                        
                        al_draw_tinted_bitmap(pokemon[i].image.bitmap,al_map_rgb(gray,gray,gray),120*(x+1),130*(y+1),0);
                        al_draw_rectangle(120*(x+1),130*(y+1),pokemon[i].image.Rect.w + 120*(x+1),
                        pokemon[i].image.Rect.h + 130*(y+1),blackSteady,3);
                        x++;
                        if(x==9){
                            x=0;
                            y++;
                        }
                    }
                }
                auxX = pokemonGridPicker[pokSelected.y-1][pokSelected.x-1];
                x = pokSelected.x -1;
                y = pokSelected.y -1;
                al_draw_rectangle(120*pokSelected.x,130*pokSelected.y,
                pokemon[auxX].image.Rect.w + (120*(x+1)),pokemon[auxX].image.Rect.h + (130*(y+1)),
                    al_map_rgb(45,100,241),8);
                if(auxX<=151 && pokemonGridPicker[pokSelected.y-1][pokSelected.x-1]!= -1){
                    if(al_key_down(&keyState,ALLEGRO_KEY_RIGHT) && pokSelected.x <=9){
                        printf("You pressed right\n");
                        pokSelected.x +=1;
                        al_rest(.13);
                    }else if(al_key_down(&keyState,ALLEGRO_KEY_LEFT)&& pokSelected.x >=2){
                        printf("You pressed left\n");
                        pokSelected.x -=1;
                        al_rest(.13);
                        //itChanged = true;
                    }else if(al_key_down(&keyState,ALLEGRO_KEY_UP) && pokSelected.y >=2){
                        printf("You pressed up\n");
                        pokSelected.y -=1;
                        al_rest(.13);
                        //itChanged = true;
                    }else if(al_key_down(&keyState,ALLEGRO_KEY_DOWN) && pokSelected.y <=2){
                        printf("You pressed down\n");
                        pokSelected.y +=1;
                        al_rest(.13);
                        //itChanged = true;
                    }
                }else{
                    pokSelected.x = 1;
                    pokSelected.y = 1;
                }
                al_draw_rectangle(120,580,1160,700,al_map_rgb(0,0,0),4);
                al_draw_text(roboto1,blackSteady,130,590,0,"NAME:");
                al_draw_text(roboto1,blackSteady,130,620,0,"TYPE:");
                al_draw_text(roboto1,blackSteady,130,650,0,"ATTACK:");
                al_draw_text(roboto1,blackSteady,730,590,0,"DEFENSE:");
                al_draw_text(roboto1,blackSteady,730,620,0,"VELOCITY:");
                al_draw_text(roboto1,blackSteady,730,650,0,"LEVEL:");
                al_draw_text(roboto1,blackSteady,330,590,0,pokemon[auxX].name);
                al_draw_text(roboto1,blackSteady,330,620,0,pokemon[auxX].type);
                char buff[10];
                sprintf(buff,"%d",pokemon[auxX].at);
                al_draw_text(roboto1,blackSteady,330,650,0,buff);
                sprintf(buff,"%d",pokemon[auxX].def);
                al_draw_text(roboto1,blackSteady,930,590,0,buff);
                sprintf(buff,"%d",pokemon[auxX].vel);
                al_draw_text(roboto1,blackSteady,930,620,0,buff);
                sprintf(buff,"%d",pokemon[auxX].lvl);
                al_draw_text(roboto1,blackSteady,930,650,0,buff);
                al_draw_scaled_bitmap(pokemon[auxX].image.bitmap,0,0,pokemon[auxX].image.Rect.w,pokemon[auxX].image.Rect.h,1030,590,100,100,0);
                if(al_key_down(&keyState,ALLEGRO_KEY_ENTER) && cnt<3 &&
                !pokemon[auxX].isChoosed){
                    ash.p[cnt]= pokemon[auxX];
                    pokemon[auxX].isChoosed = true;
                    cnt++;
                    al_rest(.17);
                }
            }else{
                al_clear_to_color(al_map_rgb(0,0,0));
                al_draw_scaled(background,0,0); 
                al_draw_text(shade,blackSteady,SCREEN_WIDTH/2-250,SCREEN_HEIGHT/2,0,"ARE YOU READY? CATCH'EM ALL");
                al_flip_display();
                al_rest(2);
                running = false;
            }
            al_flip_display();
            draw = false;
        }        
    }
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_font(roboto1);
    al_destroy_font(shade);
    al_destroy_font(NAME);
    al_destroy_display(window);
    return 0;
}