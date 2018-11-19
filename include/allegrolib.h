#ifndef ALLLIB_H_
#define ALLLIB_H_

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/timer.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#define al_texture ALLEGRO_BITMAP
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


typedef struct{
    float x;
    float y;
    int w;
    int h;
}al_Rect;

typedef struct{
    al_texture *bitmap;
    al_Rect Rect;
}al_image;

extern al_image al_load_image(char *Path);

extern void al_draw_scaled(al_image image,int dx,int dy);

extern void al_draw_character(al_image image, int si,int fi, int c, bool center);

extern void allegro_start(ALLEGRO_DISPLAY **window);

extern void register_event_sources(ALLEGRO_DISPLAY *window,ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE **event_queue);

extern void al_draw_scaled_fixed(al_image image, float dx, float dy, float sx, float sy);

extern void al_draw_scale_bitmap_region(al_image image,float sx,float sy,float w,float h, float dx, float dy,float ex,float ey);


#endif