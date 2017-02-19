#pragma once
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
ALLEGRO_BITMAP *ninja = NULL;
ALLEGRO_BITMAP *ninja_one = NULL;

void render_bitmap(int x, int y){
    al_draw_bitmap(ninja_one, x, y, 0);
}

void render(){

    int frame = 1;

    al_init();
    display = al_create_display(WIDTH, HEIGHT);
    al_init_image_addon();
    ninja = al_load_bitmap("ninja.png");
    ninja_one = al_create_sub_bitmap(ninja , TILE_WIDTH , TILE_HEIGHT , TILE_WIDTH , TILE_HEIGHT);
    al_set_target_bitmap(al_get_backbuffer(display));
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();


    while (true) {

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        ALLEGRO_EVENT ev;
        al_get_next_event(event_queue, &ev);
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { break; }
        al_clear_to_color(al_map_rgb(0,0,0));
        
        ninja(frame);
        frame++;

        al_flip_display();
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
        sleep((1000000 - delta_us) / 1000000);
    }
 
    al_destroy_bitmap(ninja_one);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
}