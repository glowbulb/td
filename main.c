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

void render (data_t a){
    ninja(a);
    sleep(5);
    a.i += 1;
    render(a);
}

int main(int argc, char **argv) {

    al_init();
    display = al_create_display(WIDTH, HEIGHT);
    al_init_image_addon();
    ninja_img = al_load_bitmap("ninja.png");

    data_t ninja_data = { .i = 1, .c = "ninja", .b = al_create_sub_bitmap(ninja_img , 0 , 0 , 32 , 32) };

    al_set_target_bitmap(al_get_backbuffer(display));

    ninja(ninja_data);

    al_destroy_display(display);

    return 0;
}
