#define _GNU_SOURCE
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CURRENT_KEYS 4

// CREATE TABLE ninja (frame INT PRIMARY KEY, id INT xpos INT, ypos INT, life INT);
// INSERT INTO ninja VALUES(1, 1, 10, 10, 100);

// CREATE TABLE input (frame INT PRIMARY KEY, b1 INT, b2 INT, b3 INT, b4 INT);
// INSERT INTO input VALUES(1, 04, 00, 00, 00);

// CREATE TABLE input_lookup (hash INT PRIMARY KEY, motion INT);
// INSERT INTO input_lookup VALUES(1000, 02);

sqlite3 *db;
char *err_msg = 0;

struct ninja_t {
    int frame;
    int id;
    int xpos;
    int ypos;
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

void ninja(){

}

int input_hash(enum input_t *a){
    int b = 0;
    for(int i = 0; i < CURRENT_KEYS; i++){
        b += a[i];
    }
    return b;
}

enum motion_t input_translate(int a){
    char *q = "";
    asprintf(&q,"SELECT * FROM input_lookup WHERE hash = %d", a);
    sqlite3_stmt *r;
    sqlite3_open("td.db", &db);
    sqlite3_prepare_v2(db, q, -1, &r, 0);
    sqlite3_step(r);
    enum motion_t e = sqlite3_column_int(r, 1);
    sqlite3_finalize(r);
    sqlite3_close(db);
    return e;
}

int input_select(int a){
    char *q = "";
    asprintf(&q,"SELECT * FROM input WHERE frame = %d", a);
    sqlite3_stmt *r;
    sqlite3_open("td.db", &db);
    sqlite3_prepare_v2(db, q, -1, &r, 0);
    sqlite3_step(r);
    static enum input_t e[CURRENT_KEYS];
    for(int i = 0; i < CURRENT_KEYS; i++){
        e[i] = sqlite3_column_int(r, i + 1);
    }
    sqlite3_finalize(r);
    sqlite3_close(db);
    return input_hash(e);
}

void ninja_move(enum motion_t a){
    if(a == WALK_RIGHT){
    }
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
        printf("%d %d %d %d %d\n", a[i].frame, a[i].id, a[i].xpos, a[i].ypos, a[i].life);
    }
}

void ninja_print(struct ninja_t a){
    printf("%d %d %d %d %d\n", a.frame, a.id, a.xpos, a.ypos, a.life);
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
        e[i].life = sqlite3_column_int(r, 4);
        i++;
    }
    ninja_store_length(e, i);
    sqlite3_finalize(r);
    sqlite3_close(db);
    return e;
}

char * ninja_insert(struct ninja_t *a, int b){
    char *x = "";
    for (int i = 0; i < b; i++) {
        char *z;
        asprintf(&z,"INSERT INTO monsters VALUES(%d, %d, %d, %d, %d);", a[i].frame, a[i].id, a[i].xpos, a[i].ypos, a[i].life);
        asprintf(&x,"%s%s", x, z);
        free(z);
    }
    return x;
}

char * ninja_create(){
    char *z;
    asprintf(&z,"CREATE TABLE ninja (frame INT PRIMARY KEY, id INT xpos INT, ypos INT, life INT);");
    return z;
}

char * ninja_select_all(){
    char *z = "SELECT * FROM ninja;";
    return z;
}