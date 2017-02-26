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
} entity_s_t;

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

typedef struct {
    entity_s_t *a;
    int s;
} entity_t;

typedef entity_s_t (*entity_map_t)(entity_s_t a, data_t b);
typedef void (*entity_foreach_t)(entity_s_t a, data_t b);

data_t entity_input_motion(data_t a){
    char *q;
    asprintf(&q,"SELECT input_lookup.motion FROM input INNER JOIN input_lookup ON input.hash = input_lookup.hash WHERE frame = %d;", a.i);
    sqlite3_stmt *r;
    sqlite3_open("td.db", &db);
    sqlite3_prepare_v2(db, q, -1, &r, 0);
    sqlite3_step(r);
    data_t e = { 
        .i = a.i,
        .c = a.c,
        .m = sqlite3_column_int(r, 0),
        .b = a.b
    };
    sqlite3_finalize(r);
    sqlite3_close(db);
    return e;
}

void entity_print_f(entity_s_t a){
    printf("%d %d %d %d %d %d %d\n", a.frame, a.id, a.xpos, a.ypos, a.img, a.life, a.speed);
}

entity_t entity_map(entity_t a, data_t b, entity_map_t c){
    entity_t d = { .a = malloc(a.s * sizeof(entity_s_t)), .s = a.s };
    for(int i = 0; i < a.s; i++){
        d.a[i] = c(a.a[i], b);
    }
    return d;
}

void entity_foreach(entity_t a, data_t b, entity_foreach_t c){
    for(int i = 0; i < a.s; i++){
        c(a.a[i], b);
    }
}

entity_t entity_return(char *a){
    sqlite3_stmt *r;
    int i = 0;
    sqlite3_open("td.db", &db);
    sqlite3_prepare_v2(db, a, -1, &r, 0);
    while (sqlite3_step(r) == SQLITE_ROW){ i++; }
    entity_t e = { .a = malloc(i * sizeof(entity_s_t)), .s = i };
    i = 0;
    while (sqlite3_step(r) == SQLITE_ROW){
        e.a[i].frame = sqlite3_column_int(r, 0);
        e.a[i].id = sqlite3_column_int(r, 1);
        e.a[i].xpos = sqlite3_column_int(r, 2);
        e.a[i].ypos = sqlite3_column_int(r, 3);
        e.a[i].img = sqlite3_column_int(r, 4);
        e.a[i].life = sqlite3_column_int(r, 5);
        e.a[i].speed = sqlite3_column_int(r, 6);
        i++;
    }
    sqlite3_finalize(r);
    sqlite3_close(db);
    return e;
}

int input_hash(input_keyboard_t *a){
    int b = 0;
    for(int i = 0; i < CURRENT_KEYS; i++){ b += a[i]; }
    return b;
}

void entity_insert_f(entity_s_t a, data_t b){
    sqlite3_stmt *r;
    char *x = "";
    asprintf(&x, "INSERT INTO %s VALUES(%d, %d, %d, %d, %d, %d, %d);", b.c, b.i + 1, a.id, a.xpos, a.ypos, a.img, a.life, a.speed);
    sqlite3_open("td.db", &db);
    sqlite3_prepare_v2(db, x, -1, &r, 0);
    sqlite3_step(r);
    sqlite3_finalize(r);
    sqlite3_close(db);
}

entity_s_t entity_move(entity_s_t a, data_t b){
    if(b.m == WALK_RIGHT){ a.xpos += a.speed; }
    if(b.m == WALK_LEFT){ a.xpos -= a.speed; }
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

void entity_draw_f(entity_s_t a, data_t b){
    al_draw_bitmap(b.b, a.xpos, a.ypos, 0);
    al_flip_display();
}

entity_foreach_t entity_draw = entity_draw_f;
entity_foreach_t entity_insert = entity_insert_f;