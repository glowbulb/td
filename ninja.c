#define _GNU_SOURCE
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ninja.h"

struct ninja_t z[5] = {
    {1,1,10,10,100},
    {1,2,20,20,100},
    {1,3,30,30,100},
    {1,4,40,40,100},
    {1,5,50,50,100}
};

int main(int argc, char **argv) {

//    db_query(db_queue_three(db_create_monsters(), db_insert_monsters(monsters, 5), db_select_table("monsters")));

    struct ninja_t *r = ninja_return(ninja_select_all());

    ninja_print_array(r);

    return 0;
}