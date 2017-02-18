#define _GNU_SOURCE
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

// create table ninja (frame integer PRIMARY KEY, id integer, xpos integer, ypos integer, life integer);
// insert into ninja values(1, 1, 10, 10, 100);

sqlite3 *db;
char *err_msg = 0;

struct ninja_t {
    int frame;
    int id;
    int xpos;
    int ypos;
    int life;
};

void store_length(struct ninja_t *a, int b){
    memcpy(a - 8, &b, 8);
}

int get_length(struct ninja_t *a){
    int *x = malloc(sizeof(int));
    memcpy(x, a - 8, 8);
    return *x;
}

void ninjas_print(struct ninja_t *a){
    for (int i = 0; i < get_length(a); i++) {
        printf("%d %d %d %d %d\n", a[i].frame, a[i].id, a[i].xpos, a[i].ypos, a[i].life);
    }
}

void ninja_print(struct ninja_t a){
    printf("%d %d %d %d %d\n", a.frame, a.id, a.xpos, a.ypos, a.life);
}

int db_callback(void *a, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i]);
    }
    return 0;
}

void db_query(char *q) {
    sqlite3_open(":memory:", &db);
    sqlite3_exec(db, q, db_callback, 0, &err_msg);
    sqlite3_close(db);
}

struct ninja_t * db_select_ninja(char *a){
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
    store_length(e, i);
    sqlite3_finalize(r);
    sqlite3_close(db);
    return e;
}

char * db_insert_ninja(struct ninja_t *a, int b){
    char *x = "";
    for (int i = 0; i < b; i++) {
        char *z;
        asprintf(&z,"INSERT INTO monsters VALUES(%d, %d, %d, %d, %d);", a[i].frame, a[i].id, a[i].xpos, a[i].ypos, a[i].life);
        asprintf(&x,"%s%s", x, z);
        free(z);
    }
    return x;
}

char * db_create_ninja(){
    char *z;
    asprintf(&z,"CREATE TABLE monsters(name TEXT, life INT, xpos INT, ypos INT);");
    return z;
}

char * db_queue_two(char *a, char *b){
    char *z;
    asprintf(&z,"%s%s",a,b);
    return z;
}

char * db_queue_three(char *a, char *b, char *c){
    char *z;
    asprintf(&z,"%s%s%s",a,b,c);
    return z;
}

char * db_create_table(char *a, char *b, char *c, char *d, char *e){
    char *z;
    asprintf(&z,"CREATE TABLE %s(%s TEXT, %s INT, %s INT, %s INT);", a, b, c, d, e);
    return z;
}

char * db_insert_row(char *a, char *b, int c, int d, int e){
    char *z;
    asprintf(&z,"INSERT INTO %s VALUES(%s, %d, %d, %d);", a, b, c, d, e);
    return z;
}

char * db_select_table(char *a){
    char *z;
    asprintf(&z,"SELECT * FROM %s;", a);
    return z;
}