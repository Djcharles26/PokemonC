#include "allegrolib.h"

void allegro_start(ALLEGRO_DISPLAY **window){
    if(!al_init())fprintf(stderr,"ERROR INIT ALLEGRO\n");
    if(!al_init_image_addon())fprintf(stderr,"ERRROR INITIALIZING IMAGE ADDON\n");
    if(!al_init_font_addon())fprintf(stderr,"ERROR INITIALIZING FONTS\n");
    if(!al_init_ttf_addon())fprintf(stderr,"ERROR INTIALIZING TTF ADDON\n");
    if(!al_init_primitives_addon())fprintf(stderr,"ERROR INITIALIZING PRIMITIVES\n");
    if(!(*window = al_create_display(SCREEN_WIDTH,SCREEN_HEIGHT)))fprintf(stderr,"ERROR CREATING DISPLAY\n");
    if(!al_install_mouse())fprintf(stderr,"ERROR INITAILIZING MOUSE\n");
    if(!al_install_keyboard())fprintf(stderr,"ERROR INITIALIZING KEYBOARD\n");
}

void register_event_sources(ALLEGRO_DISPLAY *window,ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE **event_queue){
    al_register_event_source(*event_queue,al_get_display_event_source(window));
    al_register_event_source(*event_queue,al_get_keyboard_event_source());
    al_register_event_source(*event_queue,al_get_mouse_event_source());
    al_register_event_source(*event_queue,al_get_timer_event_source(timer));
}



al_image al_load_image(char *Path){
    al_image image;
    image.bitmap = al_load_bitmap(Path);
    if(!image.bitmap){
        fflush(stdout);
        printf("Cant load texture\n");
    }
    image.Rect.x = image.Rect.y = 0;
    image.Rect.w = al_get_bitmap_width(image.bitmap);
    image.Rect.h = al_get_bitmap_height(image.bitmap);
    return image;
}

void al_draw_scaled(al_image image,int dx,int dy){
    al_draw_scaled_bitmap(image.bitmap,image.Rect.x,image.Rect.y,image.Rect.w,image.Rect.h,dx,dy,SCREEN_WIDTH,SCREEN_HEIGHT,0);
}

void al_draw_character(al_image image,int  si, int fi,int c,bool center){
    int centered;
    if(center){
        switch(si){
            case 0:
                centered = 410;
            break;
            case 1:
                centered = 130;
            break;
            case 2:
                centered = -200;
            break;
            case 3:
                centered = -430;
            break;
        }
    }else{
        centered = 0;
    }
    //draw(imagen,apuntador de imagen en X, apuntador de imagen en Y,w de imagen a imprimir,
    //h de imagen a imprimir, color, rot X,rot Y, destino en X, destino en Y,Escala en X,
    //Escala en Y,angulo ,banderas)
    for(int i=si; i<fi; i++){
        al_draw_tinted_scaled_rotated_bitmap_region(image.bitmap,(10 + 105)*i,6+(115 *c),45,75,
        al_map_rgb(255,255,255),0,0,140*(i+1 + 1*i)+centered,120,4,3.8,0,0);
        //al_draw_bitmap_region(image.bitmap,(10 + 105)*i,6,45,75,140*(i+1),120,0);
    }
}

void al_draw_scale_bitmap_region(al_image image,float sx,float sy,float w,float h, float dx, float dy,float ex,float ey){
    al_draw_tinted_scaled_rotated_bitmap_region(image.bitmap,sx,sy,
    w,h,al_map_rgb(255,255,255),0,0,dx,dy,ex,ey,0,0);
}

void al_draw_scaled_fixed(al_image image, float dx, float dy, float sx, float sy){
    al_draw_scaled_bitmap(image.bitmap,0,0,image.Rect.w,
    image.Rect.h,dx,dy,sx,sy,0);
}