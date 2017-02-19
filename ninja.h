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

// CREATE TABLE ninja (frame INT PRIMARY KEY, id INT, xpos INT, ypos INT, img INT, life INT);
// INSERT INTO ninja VALUES(1, 1, 10, 10, 1, 100);

// CREATE TABLE input (frame INT PRIMARY KEY, hash INT);
// INSERT INTO input VALUES(1, 1000);

// CREATE TABLE input_lookup (hash INT PRIMARY KEY, motion INT);
// INSERT INTO input_lookup VALUES(1000, 02);

sqlite3 *db;
char *err_msg = 0;

struct ninja_t {
    int frame;
    int id;
    int xpos;
    int ypos;
    int img;
    int life;
};

enum motion_t {
    NONE = 0,
    STAND = 1,
    WALK_RIGHT = 2,
    WALK_LEFT = 3,
    RUN_RIGHT = 4,
    RUN_LEFT = 5,
    BUMP_RIGHT = 6,
    BUMP_LEFT = 7
};

enum input_t {
    NO = 0,
    UP = 1,
    DOWN = 10,
    LEFT = 100,
    RIGHT = 1000,
    A = 10000,
    B = 100000
};

enum motion_t ninja_input_motion(int a){
    char *q = "";
    asprintf(&q,"SELECT input_lookup.motion FROM input INNER JOIN input_lookup ON input.hash = input_lookup.hash WHERE frame = %d;", a);
    sqlite3_stmt *r;
    sqlite3_open("td.db", &db);
    sqlite3_prepare_v2(db, q, -1, &r, 0);
    sqlite3_step(r);
    enum motion_t e = sqlite3_column_int(r, 0);
    sqlite3_finalize(r);
    sqlite3_close(db);
    return e;
}

void ninja_store_length(struct ninja_t *a, int b){
    memcpy(a - 8, &b, 8);
}

int ninja_get_length(struct ninja_t *a){
    int *x = malloc(sizeof(int));
    memcpy(x, a - 8, 8);
    return *x;
}

void ninja_print_array(struct ninja_t *a){
    for (int i = 0; i < ninja_get_length(a); i++) {
        printf("%d %d %d %d %d %d\n", a[i].frame, a[i].id, a[i].xpos, a[i].ypos, a[i].img, a[i].life);
    }
}

void ninja_print(struct ninja_t a){
    printf("%d %d %d %d %d %d\n", a.frame, a.id, a.xpos, a.ypos, a.img, a.life);
}

struct ninja_t * ninja_return(char *a){
    sqlite3_stmt *r;
    int i = 0;
    sqlite3_open("td.db", &db);
    sqlite3_prepare_v2(db, a, -1, &r, 0);
    while (sqlite3_step(r) == SQLITE_ROW){ i++; }
    struct ninja_t *e = malloc(i * sizeof(struct ninja_t));
    i = 0;
    while (sqlite3_step(r) == SQLITE_ROW){
        e[i].frame = sqlite3_column_int(r, 0);
        e[i].id = sqlite3_column_int(r, 1);
        e[i].xpos = sqlite3_column_int(r, 2);
        e[i].ypos = sqlite3_column_int(r, 3);
        e[i].img = sqlite3_column_int(r, 4);
        e[i].life = sqlite3_column_int(r, 5);
        i++;
    }
    ninja_store_length(e, i);
    sqlite3_finalize(r);
    sqlite3_close(db);
    return e;
}

int input_hash(enum input_t *a){
    int b = 0;
    for(int i = 0; i < CURRENT_KEYS; i++){
        b += a[i];
    }
    return b;
}

char * ninja_insert_array(struct ninja_t *a){
    char *x = "";
    for (int i = 0; i < ninja_get_length(a); i++) {
        char *z;
        asprintf(&z,"INSERT INTO ninja VALUES(%d, %d, %d, %d, %d, %d);", a[i].frame, a[i].id, a[i].xpos, a[i].ypos, a[i].img, a[i].life);
        asprintf(&x,"%s%s", x, z);
        free(z);
    }
    return x;
}

void ninja_insert(struct ninja_t a){
    sqlite3_stmt *r;
    char *x = "";
    asprintf(&x, "INSERT INTO ninja VALUES(%d, %d, %d, %d, %d, %d);", (a.frame + 1), a.id, a.xpos, a.ypos, a.img, a.life);
    sqlite3_open("td.db", &db);
    sqlite3_prepare_v2(db, x, -1, &r, 0);
    sqlite3_step(r);
    sqlite3_finalize(r);
    sqlite3_close(db);
    free(x);
}

struct ninja_t ninja_move(struct ninja_t a, enum motion_t b){
    if(b == WALK_RIGHT){
        a.xpos += 10;
    }
    if(b == WALK_LEFT){
        a.xpos -= 10;
    }
    return a;
}

char * ninja_create(){
    char *z;
    asprintf(&z,"CREATE TABLE ninja (frame INT PRIMARY KEY, id INT xpos INT, ypos INT, img INT, life INT);");
    return z;
}

char * ninja_select_all(){
    char *z = "SELECT * FROM ninja;";
    return z;
}

char * ninja_select_frame(int a){
    char *z;
    asprintf(&z,"SELECT * FROM ninja WHERE frame = %d;", a);
    return z;
}

void ninja(int a, ALLEGRO_BITMAP *b){

    struct ninja_t *c = ninja_return(ninja_select_frame(a));

    for(int i = 0; i < ninja_get_length(c); i++){
        al_draw_bitmap(b, c[i].xpos, c[i].ypos, 0);
        al_flip_display();
    }

    for(int i = 0; i < ninja_get_length(c); i++){
        ninja_insert(ninja_move(c[i], ninja_input_motion(a)));
    }
}