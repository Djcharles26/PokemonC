#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/timer.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define al_texture ALLEGRO_BITMAP

void allegro_start(ALLEGRO_DISPLAY **window){
    if(!al_init())fprintf(stderr,"ERROR INIT ALLEGRO\n");
    if(!al_init_image_addon())fprintf(stderr,"ERRROR INITIALIZING IMAGE ADDON\n");
    if(!al_init_font_addon())fprintf(stderr,"ERROR INITIALIZING FONTS\n");
    if(!al_init_ttf_addon())fprintf(stderr,"ERROR INTIALIZING TTF ADDON\n");
    if(!(*window = al_create_display(SCREEN_WIDTH,SCREEN_HEIGHT)))fprintf(stderr,"ERROR CREATING DISPLAY\n");
    if(!al_install_mouse())fprintf(stderr,"ERROR INITAILIZING MOUSE\n");
    if(!al_install_keyboard())fprintf(stderr,"ERROR INITIALIZING KEYBOARD\n");
}


void characterChoose(ALLEGRO_FONT *roboto1){

    
}


int main(void){
    int i;
    int colorNumber = 0;
    ALLEGRO_DISPLAY *window;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_USTR *input = al_ustr_new("");
    float frameTime = 0,deltaTime = 1.0/60;
    al_texture *start = NULL, *background = NULL;
    allegro_start(&window);
    bool running = true, isBlack = true,enter= false,draw = false;
    event_queue = al_create_event_queue();
    if(!event_queue){
        fprintf(stderr,"failed creating event queue\n");
        //al_destroy_bitmap(menu);
        al_destroy_display(window);
        //al_destroy_timer(timer);
        return -1;
    }
    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_TIMER *timer = al_create_timer(1.0/60);
    al_register_event_source(event_queue,al_get_display_event_source(window));
    al_register_event_source(event_queue,al_get_keyboard_event_source());
    al_register_event_source(event_queue,al_get_mouse_event_source());
    al_register_event_source(event_queue,al_get_timer_event_source(timer));
    start = al_load_bitmap("./resources/Stadiumstart.jpg");
    ALLEGRO_FONT  *roboto1 =  al_load_ttf_font("./resources/Fonts/Roboto-Bold.ttf",23,0);
    ALLEGRO_FONT  *shade = al_load_ttf_font("./resources/Fonts/RobotoCondensed-Bold.ttf",24,0);
    if(!roboto1)fprintf(stderr,"Couldnt create font\n");
    if(!shade)fprintf(stderr,"Couldnt create Shader");
    background = al_load_bitmap("./resources/psyBack.jpg");
    al_start_timer(timer);
    if(!start){
        fprintf(stderr,"Error creating bitmap\n%d\n",al_get_errno());
        return -1;
    }
    while(running){
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue,&ev);
        
        
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            printf("I ended");
            running = false;
        }else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
            if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                running = false;
            }else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER && !enter){
                printf("You pressed enter");
                enter = true;
            }
        }else if(ev.type == ALLEGRO_EVENT_TIMER){
            frameTime += deltaTime;
            if(frameTime >= 1){
                frameTime = 0;
                if(isBlack){
                    isBlack = false;
                    colorNumber = 255;
                }else{
                    isBlack = true;
                    colorNumber = 0;
                }
            }
            draw = true;

        }
        if(draw && al_event_queue_is_empty(event_queue)){
            if(!enter){
                al_draw_bitmap(start,13,1,0);
                al_draw_text(shade,al_map_rgba(colorNumber,colorNumber,colorNumber,255),SCREEN_WIDTH/2-103, SCREEN_HEIGHT/2 +200,0,"PRESS ENTER TO START");
            }else{
                al_clear_to_color(al_map_rgba(0,0,0,255));
                
                al_draw_scaled_bitmap(background,0,0,
                    al_get_bitmap_width(background),al_get_bitmap_height(background),
                    0,0,SCREEN_WIDTH,SCREEN_HEIGHT,0);
                    characterChoose(roboto1);
            }
            al_flip_display();
            draw = false;
        }

        //al_clear_to_color(al_map_rgb(0,0,0));
        
    }

    al_destroy_timer(timer);
    al_destroy_bitmap(start);
    al_destroy_font(roboto1);
    al_destroy_event_queue(event_queue);
    al_destroy_display(window);
    return 0;
}