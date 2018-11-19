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

extern trainer ash;

void enemyGenerator(trainer *enemy, pokeInfo pokemon[151]){
    int r = rand()%9;
    switch(r){
        case 0:
            strcpy(enemy->name,"Michael");
        break;
        case 1:
            strcpy(enemy->name,"Louis");

        break;
        case 2:
            strcpy(enemy->name,"Rauner");
        break;
        case 3:
            strcpy(enemy->name,"Mel");
        break;
        case 4:
            strcpy(enemy->name,"Ann");
        break;
        case 5:
            strcpy(enemy->name,"Ali");
        break;
        case 6:
            strcpy(enemy->name,"Zame");
        break;
        case 7:
            strcpy(enemy->name,"Elcu");
        break;
        case 8:
            strcpy(enemy->name,"Lore");
        break;
    }
    enemy->ch = rand()%4;
    enemy->lvl = rand()%3;
    for(int i=0; i<3; i++){
        enemy->p[i] = pokemon[rand()%10];
    }
}

int game(pokeInfo pokemon[151]){
    srand(time(NULL));
    ALLEGRO_DISPLAY *window = NULL;
    allegro_start(&window);
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT ev;
    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *font1 = al_load_font("./resources/Fonts/Early-GameBoy.ttf",23,0);
    ALLEGRO_FONT *font2 = al_load_font("./resources/Fonts/Early-GameBoy.ttf",46,0);
    ALLEGRO_FONT *font3 = al_load_font("./resources/Fonts/Early-GameBoy.ttf",100,0);
    al_image background = al_load_image("./resources/back_pokemon.png");
    al_image characters = al_load_image("./resources/trainer_sprites.png");
    al_image battleBackground = al_load_image ("./resources/battleBackground.jpg");
    al_image multipleBattle = al_load_image("./resources/multipleBackgrounds.png");
    float frameTime = 0,chargeTime=0, deltaTime = 1.0/60, battleCharge=0;
    bool running = true,draw = false,beated = true,dots=false,stopDots = false,battlePresent = true;
    bool enemyDisplay = false, start = false;
    int cnt=0,xback=0,yback=0;
    coords selectMove;
    trainer enemy;
    if(!font1 || !font2 || !font3){
        fprintf(stderr,"ERROR CREATING FONTS\n");
        return -1;
    }
    if(!(event_queue = al_create_event_queue()))fprintf(stderr,"ERROR CREATING EVENT QUEUE\n");
    timer = al_create_timer(1.0/60);
    register_event_sources(window,timer,&event_queue);
    al_start_timer(timer);
    while(running){
        al_get_keyboard_state(&keyState);
        al_wait_for_event(event_queue,&ev);
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            running = false;
        }else if(al_key_down(&keyState,ALLEGRO_KEY_ESCAPE)){
            running = false;
        }else if(ev.type == ALLEGRO_EVENT_TIMER){
            frameTime += deltaTime;
            if(frameTime >= 1){
                frameTime = 0;
            }
            if(start){
                battleCharge = battleCharge +  deltaTime; + (deltaTime * (cnt-1));
                if(battleCharge >= .05){
                    if(battlePresent){
                        cnt++;
                    }else{
                        cnt--;
                    }
                    battleCharge = 0; 
                }
            }
            if(dots){
                chargeTime += deltaTime;
                if(chargeTime>=1){
                    chargeTime = 0;
                    cnt++;
                }
                if(cnt==0){
                    al_draw_text(font2,al_map_rgb(0,0,0),SCREEN_WIDTH/2-45,SCREEN_HEIGHT/2+40,0,".");
                }else if(cnt==1){
                    al_draw_text(font2,al_map_rgb(0,0,0),SCREEN_WIDTH/2-45,SCREEN_HEIGHT/2+40,0,". .");
                }else if(cnt==2){
                    al_draw_text(font2,al_map_rgb(0,0,0),SCREEN_WIDTH/2-45,SCREEN_HEIGHT/2+40,0,". . .");
                }
                if(cnt==3){
                    enemyDisplay = true;
                    stopDots = true;
                    dots = false;
                    cnt = 0;
                }
                al_flip_display();
            }
            draw = true;
        }
        if(beated){
            enemyGenerator(&enemy,pokemon);
            beated = false;
            xback = rand()%3;
            yback = rand()%3;
        }

        
        if(draw && al_event_queue_is_empty(event_queue)){
            al_clear_to_color(al_map_rgb(0,0,0));
            //al_draw_bitmap(background.bitmap,0,0,0);
            //al_draw_bitmap_region(background.bitmap,20,50,255,255,20,20,0);
            if(battlePresent){
                al_draw_scale_bitmap_region(background,1146,27,255,255,0,0,5.01,2.82);
                al_draw_text(font2,al_map_rgb(0,0,0),SCREEN_WIDTH/2-400,SCREEN_HEIGHT/2-300,0,"YOUR ENEMY WILL BE:");
                if(!dots && !stopDots){
                    dots = true;
                }else if(enemyDisplay){
                    al_draw_text(font2,al_map_rgb(0,0,0),SCREEN_WIDTH/2-105,SCREEN_HEIGHT/2+40,0,enemy.name);
                    al_draw_character(characters,enemy.ch,enemy.ch+1,1,true);
                    al_draw_text(font1,al_map_rgb(0,0,0),SCREEN_WIDTH/2+50,680,0,"PRESS ENTER TO CONTINUE");
                    if(al_key_down(&keyState,ALLEGRO_KEY_ENTER) && !start){
                        start= true;
                    }
                    if(start){
                        al_draw_line(640,0,640,720,al_map_rgb(0,0,0),12*cnt);
                        if(cnt>140){
                            //enemyDisplay = false;
                            battlePresent = false;
                        }
                    }
                }
            }else{
                al_draw_scale_bitmap_region(multipleBattle,6+(244)*xback,30+(117)*yback,
                240,115,0,0,5.33,6.26);
                //al_draw_scaled_bitmap(battleBackground.bitmap,0,0,battleBackground.Rect.w,
                //battleBackground.Rect.h,0,0,1900,900,0);//1.82,1.44,0);
                if(cnt>0){
                    //al_draw_bitmap(battleBackground.bitmap,10,10,0);
                    al_draw_line(640,0,640,720,al_map_rgb(0,0,0),8*cnt);
                }
            } 
            al_flip_display();
            draw = false;       
        }
    }
    al_destroy_display(window);
    al_destroy_font(font1);
    al_destroy_font(font2);
    al_destroy_font(font3);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    
}