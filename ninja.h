#pragma once
#define _GNU_SOURCE
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "entity.h"

// ENTITY HEADER

// ENTITY FUNCTIONS


// ENTITY MAIN FUNCTION
void ninja(data_t a){

//    entity_t r = entity_return(entity_select_frame(a.i, a.c), a);

    entity_t r = entity_return(entity_select_all(a.c), a);

    entity_foreach(r, a, entity_draw);

//    entity_t s = entity_map(r, entity_input_motion(a), entity_move);

//    entity_foreach(s, a, entity_insert);
}