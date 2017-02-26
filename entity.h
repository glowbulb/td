#pragma once
#define _GNU_SOURCE
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "db.h"
#define CURRENT_KEYS 4

// CREATE TABLE ninja (frame INT, id INT, xpos INT, ypos INT, img INT, life INT, speed INT);
// INSERT INTO ninja VALUES(1, 1, 10, 10, 1, 100, 10);

// CREATE TABLE input (frame INT PRIMARY KEY, hash INT);
// INSERT INTO input VALUES(1, 1000);

// CREATE TABLE input_lookup (hash INT PRIMARY KEY, motion INT);
// INSERT INTO input_lookup VALUES(1000, 02);

sqlite3 *db;
char *err_msg = 0;

typedef struct {
    int frame;
    int id;
    int xpos;
    int ypos;
    int img;
    int life;
    int speed;
} entity_t;

typedef enum {
    RED_NINJA = 0,
    BLACK_NINJA = 1
} subtype_t;

typedef enum {
    NONE = 0,
    STAND = 1,
    WALK_RIGHT = 2,
    WALK_LEFT = 3,
    RUN_RIGHT = 4,
    RUN_LEFT = 5,
    BUMP_RIGHT = 6,
    BUMP_LEFT = 7
} motion_t;

typedef enum {
    NO = 0,
    UP = 1,
    DOWN = 2,
    LEFT = 4,
    RIGHT = 8,
    A = 16,
    B = 32
} input_keyboard_t;

typedef struct {
    int x;
    int y;
    bool left;
    bool right;
} input_mouse_t;

typedef struct {
    int i;
    char *c;
    motion_t m;
    ALLEGRO_BITMAP *b;
} data_t;

typedef entity_t (*entity_map_t)(entity_t a, data_t b);
typedef void (*entity_foreach_t)(entity_t a, data_t b);
typedef void (*entity_foreach__t)(entity_t a);

data_t entity_input_motion(data_t a){
    char *q = "";
    asprintf(&q,"SELECT input_lookup.motion FROM input INNER JOIN input_lookup ON input.hash = input_lookup.hash WHERE frame = %d;", a.i);
    sqlite3_stmt *r;
    sqlite3_open("td.db", &db);
    sqlite3_prepare_v2(db, q, -1, &r, 0);
    sqlite3_step(r);
    data_t e = { .i = sqlite3_column_int(r, 0) };
    sqlite3_finalize(r);
    sqlite3_close(db);
    return e;
}

void entity_store_length(entity_t *a, int b){
    memcpy(a - 8, &b, 8);
}

int entity_get_length(entity_t *a){
    int *x = malloc(sizeof(int));
    memcpy(x, a - 8, 8);
    return *x;
}

void entity_print_f(entity_t a){
    printf("%d %d %d %d %d %d %d\n", a.frame, a.id, a.xpos, a.ypos, a.img, a.life, a.speed);
}

entity_t * entity_map(entity_t *a, data_t b, entity_map_t c){
    const int l = entity_get_length(a);
    entity_t *d = malloc(l * sizeof(entity_t));
    for(int i = 0; i < l; i++){
        d[i] = c(a[i], b);
    }
    return d;
}

void entity_foreach(entity_t *a, data_t b, entity_foreach_t c){
    const int l = entity_get_length(a);
    for(int i = 0; i < l; i++){
        c(a[i], b);
    }
}

void entity_foreach_(entity_t *a, entity_foreach__t b){
    for(int i = 0; i < entity_get_length(a); i++){
        b(a[i]);
    }
}

entity_t * entity_return(char *a){
    sqlite3_stmt *r;
    int i = 0;
    sqlite3_open("td.db", &db);
    sqlite3_prepare_v2(db, a, -1, &r, 0);
    while (sqlite3_step(r) == SQLITE_ROW){ i++; }
    entity_t *e = malloc(i * sizeof(entity_t));
    i = 0;
    while (sqlite3_step(r) == SQLITE_ROW){
        e[i].frame = sqlite3_column_int(r, 0);
        e[i].id = sqlite3_column_int(r, 1);
        e[i].xpos = sqlite3_column_int(r, 2);
        e[i].ypos = sqlite3_column_int(r, 3);
        e[i].img = sqlite3_column_int(r, 4);
        e[i].life = sqlite3_column_int(r, 5);
        e[i].speed = sqlite3_column_int(r, 6);
        i++;
    }
    entity_store_length(e, i);
    sqlite3_finalize(r);
    sqlite3_close(db);
    return e;
}

int input_hash(input_keyboard_t *a){
    int b = 0;
    for(int i = 0; i < CURRENT_KEYS; i++){ b += a[i]; }
    return b;
}

void entity_insert_f(entity_t a, data_t b){
    sqlite3_stmt *r;
    char *x = "";
    asprintf(&x, "INSERT INTO %s VALUES(%d, %d, %d, %d, %d, %d, %d);", b.c, a.frame = a.frame + 1, a.id, a.xpos, a.ypos, a.img, a.life, a.speed);
    sqlite3_open("td.db", &db);
    sqlite3_prepare_v2(db, x, -1, &r, 0);
    sqlite3_step(r);
    sqlite3_finalize(r);
    sqlite3_close(db);
}

entity_t entity_move(entity_t a, data_t b){
    if(b.m == WALK_RIGHT){ a.xpos += 10; }
    if(b.m == WALK_LEFT){ a.xpos -= 10; }
    return a;
}

char * entity_create(char *a){
    char *z;
    asprintf(&z, "CREATE TABLE %s (frame INT PRIMARY KEY, id INT xpos INT, ypos INT, img INT, life INT, speed INT);", a);
    return z;
}

char * entity_select_all(char *a){
    char *z;
    asprintf(&z, "SELECT * FROM %s;", a);
    return z;
}

char * entity_select_frame(int a, char *b){
    char *z;
    asprintf(&z, "SELECT * FROM %s WHERE frame = %d;", b, a);
    return z;
}

void entity_draw_f(entity_t a, data_t b){
    al_draw_bitmap(b.b, a.xpos, a.ypos, 0);
    al_flip_display();
}

entity_t * entity_insert_map(entity_t *a, data_t b){
    return entity_map(a, b, entity_move);
}

entity_foreach_t entity_draw = entity_draw_f;
entity_foreach__t entity_print = entity_print_f;