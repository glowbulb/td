#pragma once
#define _GNU_SOURCE
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ninja.h"

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