#pragma once
#define _GNU_SOURCE
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "db.h"
#include "entity.h"

// ENTITY HEADER

// ENTITY FUNCTIONS


// ENTITY MAIN FUNCTION
void ninja(data_t a){
    entity_t *r = entity_return(entity_select_frame(a.i, a.c));
    entity_foreach_(r, entity_print);
//    entity_foreach(r, a, entity_draw);
//    entity_foreach(r, a, entity_insert);
}