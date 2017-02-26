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

int main(int argc, char **argv) {

    al_init();
    display = al_create_display(WIDTH, HEIGHT);
    al_init_image_addon();
    ninja_img = al_load_bitmap("ninja.png");
    al_set_new_display_option(ALLEGRO_VSYNC, 2, ALLEGRO_REQUIRE);
    al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, 1, ALLEGRO_REQUIRE);

    sqlite3 *db;
    sqlite3_open("td.db", &db);
    // sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, NULL);
    // sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, NULL);

    data_t ninja_data = {
        .i = 1,
        .c = "ninja",
        .b = al_create_sub_bitmap(ninja_img, 0, 0, 32, 32),
        .db = db
    };

    al_set_target_bitmap(al_get_backbuffer(display));

    ninja(ninja_data);

    // for( int i = 2; i < 1000; i++){
    //     ninja_data.i = i;
    //     render(ninja_data);
    // }
    

    al_destroy_display(display);
    
    sqlite3_close(db);
    
    return 0;
}
