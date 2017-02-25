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
    entity_t *r = entity_return_frame(a);
    entity_foreach(r, a, entity_draw);
    entity_foreach(entity_insert_map(r, a), a, entity_insert);
}