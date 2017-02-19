#define _GNU_SOURCE
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "ninja.h"

#define WIDTH 640
#define HEIGHT 480
#define TILE_HEIGHT 32
#define TILE_WIDTH 32
#define NINJA_SIZE 32
#define FPS 1

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *ninja_img = NULL;
ALLEGRO_BITMAP *ninja_one = NULL;
int frame = 1;

int main(int argc, char **argv) {

    al_init();
    display = al_create_display(WIDTH, HEIGHT);
    al_init_image_addon();
    ninja_img = al_load_bitmap("ninja.png");
    ninja_one = al_create_sub_bitmap(ninja_img , TILE_WIDTH , TILE_HEIGHT , TILE_WIDTH , TILE_HEIGHT);
    al_set_target_bitmap(al_get_backbuffer(display));
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));

//    struct timespec start, end;

//    ninja_test();

//        clock_gettime(CLOCK_MONOTONIC_RAW, &start);

//        frame++;
//        ALLEGRO_EVENT ev;
//        al_get_next_event(event_queue, &ev);
//        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { break; }

        ninja(frame, ninja_one);
        
//        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
//        uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;

//        sleep((1000000 - delta_us) / 1000000);

//    al_destroy_bitmap(ninja_one);

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}