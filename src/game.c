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
#define RedBallx 132
#define RedBally 65
#define RedBallw 10
#define RedBallh 10
#define RedBallxS 3.84
#define RedBallyS 9.6

bool redArrow = false;
int xArrow=0,yArrow=0;


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
    enemy->pokeQ = 3;
    for(int i=0; i<3; i++){
        enemy->p[i] = pokemon[rand()%150];
        if(!enemy->p[i].isRoot){
            i--;
        }
    }
}

int enemyStarted(ALLEGRO_FONT *font1, trainer enemy){
    al_draw_text(font1,al_map_rgb(0,0,0),80,590,0,enemy.name);
    al_draw_text(font1,al_map_rgb(0,0,0),260,590,0,"HAS SPAWNED");
    al_draw_text(font1,al_map_rgb(0,0,0),600,590,0,enemy.p[3-enemy.pokeQ].name);  
    return enemy.p[3-enemy.pokeQ].index;
}

int userStarted(ALLEGRO_FONT *font1){
    al_draw_text(font1,al_map_rgb(0,0,0),80,590,0,ash.name);
    al_draw_text(font1,al_map_rgb(0,0,0),260,590,0,"HAS SPAWNED");
    al_draw_text(font1,al_map_rgb(0,0,0),600,590,0,ash.p[3-ash.pokeQ].name); 
    return ash.p[3-ash.pokeQ].index; 
}

void SelectMovementU(int op, bool select1,ALLEGRO_FONT * font1){
    int damage;
    if(select1){
        al_draw_text(font1,al_map_rgb(0,0,0),160,590,0,"Ember");
        al_draw_text(font1,al_map_rgb(0,0,0),460,590,0,"Tackle");
        al_draw_text(font1,al_map_rgb(0,0,0),160,640,0,"Tail Whip");
        al_draw_text(font1,al_map_rgb(0,0,0),460,640,0,"Growl");
    }
}

void TextRect(al_image Arrow, ALLEGRO_FONT *font1){
    al_draw_filled_rectangle(0,550,1280,720,al_map_rgb(255,255,255));
    al_draw_rectangle(0,550,1280,720,al_map_rgb(120, 7, 146),20);
    if(redArrow){
        al_draw_scale_bitmap_region(Arrow,538,32,40,40,40 + (390 * xArrow),560 + (50 * yArrow),1.84,1.6);
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
    al_image cleanChar = al_load_image("./resources/trainer_sprites_clean.png");
    al_image battleBackground = al_load_image ("./resources/battleBackground.jpg");
    al_image multipleBattle = al_load_image("./resources/multipleBackgrounds.png");
    al_image menu = al_load_image("./resources/menu.png");
    al_image pokUser;
    al_image pokEnemy;
    float frameTime = 0,chargeTime=0, deltaTime = 1.0/60, battleCharge=0,waiterE = 0, waiterP=0;
    bool running = true,draw = false,beated = true,dots=false,stopDots = false,battlePresent = true;
    bool enemyDisplay = false, start = false,texting = false,playP=false,playE=false, eAttacked = false;
    bool eAttacking = false, userWon = false, enemyWon = false;
    bool battle = false,at,def,select1=false,select2=false, enemyCont=false, isSelecting = false;
    int cnt=0,xback=0,yback=0,text = 0,playmoveP = 0, playmoveE = 0, pokNumU, pokNumE;
    int battle_time = 0,turn, op=0,damage = 0, enemyMove = 0;
    attack attacks[2][2], attackUsed;
    int value = -1, restart = 0;
    ALLEGRO_COLOR hpColor=al_map_rgb(0,0,0),hpColor2=al_map_rgb(0,0,0); 
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
            if(frameTime >= 0.5){
                frameTime = 0;
                redArrow = !redArrow;
            }
            if(start){
                battleCharge = battleCharge +  deltaTime; + (deltaTime * (cnt-1));
                if(battleCharge >= .01){
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
                    al_draw_text(font2,al_map_rgb(0,0,0),SCREEN_WIDTH/2-70,SCREEN_HEIGHT/2+40,0,".");
                }else if(cnt==1){
                    al_draw_text(font2,al_map_rgb(0,0,0),SCREEN_WIDTH/2-70,SCREEN_HEIGHT/2+40,0,". .");
                }else if(cnt==2){
                    al_draw_text(font2,al_map_rgb(0,0,0),SCREEN_WIDTH/2-70,SCREEN_HEIGHT/2+40,0,". . .");
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
            if(playE){
                waiterE += deltaTime;
                if(waiterE>=2){
                    waiterE = 0;
                }
            }
        }
        if(beated){
            enemyGenerator(&enemy,pokemon);
            beated = false;
            xback = rand()%3;
            yback = rand()%3;
        }
        //SELECT SECTION //TODO: 
        if(select1){
            if(al_key_down(&keyState,ALLEGRO_KEY_LEFT) && xArrow == 1){
                xArrow = 0;
            }else if(al_key_down(&keyState,ALLEGRO_KEY_RIGHT) && xArrow == 0){
                xArrow = 1;
            }else if(al_key_down(&keyState,ALLEGRO_KEY_DOWN) && yArrow == 0){
                yArrow = 1;
            }else if(al_key_down(&keyState,ALLEGRO_KEY_UP) && yArrow == 1){
                yArrow = 0;
            }
            //Change battle Time 
            if(al_key_down(&keyState,ALLEGRO_KEY_ENTER)){
                select1=false;
                attackUsed = ash.p[3-ash.pokeQ].ataques[(2*yArrow)+xArrow];
                damage = attackUsed.damage + 7*(rand()%15 + 1) - enemy.p[3-enemy.pokeQ].def;
                if(damage>0){
                    enemy.p[3-enemy.pokeQ].hp -= damage;
                }
                al_rest(.5);
            }
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
                            texting = true;
                        }
                    }
                }
            }else if(texting){
                al_draw_scale_bitmap_region(multipleBattle,6+(244)*xback,30+(117)*yback,
                240,115,0,0,5.33,6.26);
                if(cnt>0){
                    al_draw_line(640,0,640,720,al_map_rgb(0,0,0),8*cnt);
                }else{
                    al_draw_filled_rectangle(0,550,1280,720,al_map_rgb(255,255,255));
                    al_draw_rectangle(0,550,1280,720,al_map_rgb(120, 7, 146),20);
                    al_draw_scale_bitmap_region(cleanChar,115*enemy.ch,121,45,75,930,10,4,3.8);
                    al_draw_scale_bitmap_region(menu,177.6,65.8,104,12.5,20,10,3.84,9.6);
                    al_draw_scale_bitmap_region(menu,177.6,95.3,104,12.5,800,400,3.84,9.6);
                    for(int i=0; i<3; i++){
                        al_draw_scale_bitmap_region(menu,142,65,10,10,878+38*(i),407.5,3.84,9.6);
                        al_draw_scale_bitmap_region(menu,142,65,10,10,236+38*(i),12,3.84,9.6);
                    }
                    for(int i=3  - enemy.pokeQ; i<3; i++){
                        al_draw_scale_bitmap_region(menu,132,65,10,10,236+38*(i),12,3.84,9.6);
                    }
                    for(int i=0; i<ash.pokeQ; i++){
                        al_draw_scale_bitmap_region(menu,132,65,10,10,878+38*(i),407.5,3.84,9.6);
                    }
                    if(redArrow){
                        al_draw_scale_bitmap_region(menu,538,32,40,40,40,560,1.84,1.6);
                    }
                    //al_draw_bitmap_region(menu.bitmap,190,68,100,16,20,10,0);
                    if(text == 0){  
                        al_draw_text(font1,al_map_rgb(0,0,0),80,590,0,"Trainer");
                        al_draw_text(font1,al_map_rgb(0,0,0),260,590,0,enemy.name);
                        al_draw_text(font1,al_map_rgb(0,00,0),370,590,0,"Has challenged You");
                    }else if(text == 1){
                        al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,"LETS GIVE HIM A FIGHT!");
                    }else if(text == 2){
                        char Path[100];
                        pokNumU=userStarted(font1);
                        sprintf(Path,"./resources/generation-1/red-blue/back/%d.png",pokNumU);
                        pokUser=al_load_image(Path);
                        al_draw_scaled_bitmap(pokUser.bitmap,0,0,pokUser.Rect.w,pokUser.Rect.h,200,280,250,250,0);
                    }else if(text == 3){
                        char Path[100];
                        pokNumE=enemyStarted(font1,enemy);
                        sprintf(Path,"./resources/generation-1/red-blue/%d.png",pokNumE);
                        pokEnemy=al_load_image(Path);
                        al_draw_scaled_bitmap(pokUser.bitmap,0,0,pokUser.Rect.w,pokUser.Rect.h,200,280,250,250,0);
                        al_draw_scaled_bitmap(pokEnemy.bitmap,0,0,pokEnemy.Rect.w,pokEnemy.Rect.h,850,100,120,120,0);
                    }
                    if(al_key_down(&keyState,ALLEGRO_KEY_ENTER)){
                        text++;
                        al_rest(.2);
                    }
                    if(text>=4){
                        texting = false;
                        battle = true;
                        turn = rand()%2;
                    }
                }
            }else if(battle && !enemyWon && !userWon){
                //every Time this will be drawed
                al_draw_scale_bitmap_region(multipleBattle,6+(244)*xback,30+(117)*yback,
                    240,115,0,0,5.33,6.26);
                TextRect(menu,font1);
                al_draw_scaled_bitmap(pokUser.bitmap,0,0,pokUser.Rect.w,pokUser.Rect.h,200,280,250,250,0);
                al_draw_scaled_bitmap(pokEnemy.bitmap,0,0,pokEnemy.Rect.w,pokEnemy.Rect.h,850,100,120,120,0);
                char t[100];
                if(enemy.p[3-enemy.pokeQ].hp>50){
                    hpColor2.r = hpColor2.b = 0;
                    hpColor2.g  = 255;
                }else if(enemy.p[3-enemy.pokeQ].hp<=50 && enemy.p[3-enemy.pokeQ].hp>20){
                    hpColor2.r = 154;
                    hpColor2.g = 150;
                    hpColor2.b = 50;
                }else if(enemy.p[3-enemy.pokeQ].hp<=20){
                    hpColor2.r = 255;
                    hpColor2.g = hpColor2.b = 0;
                }
                if(ash.p[3-ash.pokeQ].hp>50){
                     hpColor.g = 255;
                    hpColor.r =hpColor.b = 0;
                }else if(ash.p[3-ash.pokeQ].hp<=50 && ash.p[3-ash.pokeQ].hp>20){
                    hpColor.r = 154;
                    hpColor.g = 200;
                    hpColor.b = 50;
                }else if(ash.p[3-ash.pokeQ].hp<=20){
                    hpColor.r = 255;
                    hpColor.g = hpColor.b = 0;
                }
                sprintf(t,"%d",enemy.p[3-enemy.pokeQ].hp);
                al_draw_text(font2,hpColor2,40,100,0,t);
                sprintf(t,"%d",ash.p[3-ash.pokeQ].hp);
                al_draw_text(font2,hpColor,1100,450,0,t);
                if(battle_time==0 && enemy.p[3-enemy.pokeQ].hp>0 && ash.p[3-ash.pokeQ].hp>0){
                    if(turn%2==0){

                        if(!isSelecting){
                            select1 = true;
                            isSelecting = true;
                        }

                        if(select1){
                            SelectMovementU(op,select1,font1);
                        }else{
                            xArrow = yArrow = 0;
                            //damage = attackUsed.damage  + (attackUsed.damage * (ash.p[3-ash.pokeQ].ac/100)) - enemy.p[3-enemy.pokeQ].def *(enemy.p[3-enemy.pokeQ].def * enemy.p[3-enemy.pokeQ].vel/100);
                            printf("%d\n",damage);                            
                            if(damage <= 0){
                                al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,"It missed");  
                            }else if(damage >0 && damage <50){
                                al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,"It wasnt so effective");  
                            }else if(damage >=50){
                                al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,"It was effective");  
                            }
                            if(al_key_down(&keyState,ALLEGRO_KEY_ENTER)){
                                battle_time ++;
                                turn ++;
                                isSelecting = false;
                            al_rest(.2);
                            }
                        }
                    }else{

                        if(!eAttacked){
                            eAttacked = true;
                            eAttacking = true;
                        }

                        if(eAttacking){
                            attackUsed = enemy.p[3-enemy.pokeQ].ataques[rand()%4];
                            eAttacking  = false;
                            damage = attackUsed.damage + 7*(rand()%13 + 1) - ash.p[3-ash.pokeQ].def;
                            if(damage>0){
                                ash.p[3-ash.pokeQ].hp -= damage; 
                            }                       
                        }

                        if(enemyMove==0){
                            al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,enemy.p[3-enemy.pokeQ].name);
                            al_draw_text(font1,al_map_rgb(0,00,0),350,590,0,"Used");  
                            al_draw_text(font1,al_map_rgb(0,00,0),650,590,0,attackUsed.name);
                        }else{
                            //damage = attackUsed.damage  + (attackUsed.damage * (enemy.p[3-enemy.pokeQ].ac/100)) - (ash.p[3-ash.pokeQ].def + (ash.p[3-ash.pokeQ].def * ash.p[3-ash.pokeQ].vel/100));
                            printf("%d\n",damage);                           
                            if(damage <= 0){
                                al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,"It missed");  
                            }else if(damage >0 && damage <50){
                                    al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,"It wasnt so effective");  
                            }else if(damage >=50){
                                    al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,"It was effective");  
                            }    
                        }                    
                        if(al_key_down(&keyState,ALLEGRO_KEY_ENTER)){
                            enemyMove++;
                            if(enemyMove>1){
                                enemyMove = 0;
                                turn++;
                                battle_time++;
                                eAttacked = false;
                            }
                            al_rest(.2);
                        }
                    }
                }else if(battle_time==1 && enemy.p[3-enemy.pokeQ].hp>0 && ash.p[3-ash.pokeQ].hp>0){
                    if(turn%2==0){
                        if(!isSelecting){
                            select1 = true;
                            isSelecting = true;
                        }
                        if(select1){
                            SelectMovementU(op,select1,font1);
                        }else{
                            xArrow = yArrow = 0;
                            //damage = attackUsed.damage  + (attackUsed.damage * (ash.p[3-ash.pokeQ].ac/100)) - enemy.p[3-enemy.pokeQ].def *(enemy.p[3-enemy.pokeQ].def * enemy.p[3-enemy.pokeQ].vel/100);
                            printf("%d\n",damage);
                            if(damage <= 0){
                                al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,"It missed");  
                            }else if(damage >0 && damage <50){
                                al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,"It wasnt so effective");  
                            }else if(damage >50){
                                al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,"It was effective");  
                            }
                            if(al_key_down(&keyState,ALLEGRO_KEY_ENTER)){
                                printf("continue");
                                battle_time = 0;
                                turn ++;
                                isSelecting = false;
                            al_rest(.2);
                            }
                        }
                    }else{
                        if(!eAttacked){
                            eAttacked = true;
                            eAttacking = true;
                        }
                        if(eAttacking){
                            attackUsed = enemy.p[3-enemy.pokeQ].ataques[rand()%4];
                            damage = attackUsed.damage + 7 * (rand()%13 + 1 ) - ash.p[3-ash.pokeQ].def;
                            if(damage>0){
                                ash.p[3-ash.pokeQ].hp -= damage;
                            }
                            eAttacking  = false;
                        }
                        if(enemyMove==0){
                            al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,enemy.p[3-enemy.pokeQ].name);
                            al_draw_text(font1,al_map_rgb(0,00,0),350,590,0,"Used");  
                            al_draw_text(font1,al_map_rgb(0,00,0),650,590,0,attackUsed.name);
                        }else{
                            //damage = attackUsed.damage  + (attackUsed.damage * (enemy.p[3-enemy.pokeQ].ac/100)) - (ash.p[3-ash.pokeQ].def + (ash.p[3-ash.pokeQ].def * ash.p[3-ash.pokeQ].vel/100));
                            printf("%d\n",damage);
                            if(damage <= 0){
                                al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,"It missed");  
                            }else if(damage >0 && damage <=50){
                                    al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,"It wasnt so effective");  
                                
                            }else if(damage >50){
                                    al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,"It was effective");  
                            }    
                        }                    
                        if(al_key_down(&keyState,ALLEGRO_KEY_ENTER)){
                            enemyMove++;
                            if(enemyMove>1){
                                enemyMove = 0;
                                turn++;
                                battle_time = 0;
                                eAttacked = false;
                            }
                            al_rest(.2);
                        }
                    }

                }else{
                    if(enemy.p[3-enemy.pokeQ].hp<0){
                        enemy.pokeQ--;
                        xArrow=0;
                    }else{
                        ash.pokeQ--;
                        yArrow=0;
                    }
                    texting = true;
                    battle = false;
                    if(enemy.pokeQ==0){
                        userWon = true;
                    }else if(ash.pokeQ==0){
                        enemyWon = true;
                    }else{
                        text = 2;
                    }
                }
            }else{
                if(userWon){
                    al_draw_scale_bitmap_region(multipleBattle,6+(244)*xback,30+(117)*yback,
                    240,115,0,0,5.33,6.26);
                    TextRect(menu,font1);
                    al_draw_scaled_bitmap(pokUser.bitmap,0,0,pokUser.Rect.w,pokUser.Rect.h,200,280,250,250,0);
                    al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,"You Won");  
                }else{
                    al_draw_scale_bitmap_region(multipleBattle,6+(244)*xback,30+(117)*yback,
                    240,115,0,0,5.33,6.26);
                    TextRect(menu,font1);
                    al_draw_scaled_bitmap(pokEnemy.bitmap,0,0,pokEnemy.Rect.w,pokEnemy.Rect.h,850,100,120,120,0);
                    al_draw_text(font1,al_map_rgb(0,00,0),80,590,0,enemy.name); 
                    al_draw_text(font1,al_map_rgb(0,00,0),200,590,0,"Won"); 
                }
                if(al_key_down(&keyState,ALLEGRO_KEY_ENTER)){
                    running = false;
                }

            }
            al_flip_display();
            draw = false;       
        }
    }
    al_clear_to_color(al_map_rgb(0,0,0));
    do{
        al_draw_text(font1,al_map_rgb(255,255,255),SCREEN_WIDTH/2-300,250,0,"Continue?"); 
        al_draw_text(font1,al_map_rgb(255,255,255),SCREEN_WIDTH/2-400,350,0,"Press Y or N"); 
        if(al_key_down(&keyState,ALLEGRO_KEY_Y)){
            running = true;
            restart = 1;
        }else if(al_key_down(&keyState,ALLEGRO_KEY_N)){
            running = true;
            restart = 0;
        }
    }while(!running);
    al_destroy_display(window);
    al_destroy_font(font1);
    al_destroy_font(font2);
    al_destroy_font(font3);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    return restart;
}