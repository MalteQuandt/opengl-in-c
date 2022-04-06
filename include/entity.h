#pragma once

#include <mesh.h>
#include <shader.h>
#include <player.h>

#define INDEX_NOT_INCLUDED 10
#define ILLEGAL_ARGUMENT 1

// I have chosen to represent the entities as a linked list, as all operations should
// be done in sequence, and for that a linked list is just the perfect data structure
typedef struct entity_t {
    struct entity_t * next;
    Player_t * entity;
} Entity_t;

Entity_t * entity_init(Player_t * entity, int * error);
int entity_appendEntity(Entity_t * head, Player_t * ent);
Entity_t * entity_getnthEntity(Entity_t * head, int n, int * error);
// Apply a specific function on all entities in the list:
int entity_applyFunction(Entity_t * head, void (*proc)(Entity_t *, void*, size_t), void*, size_t);
