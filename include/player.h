#pragma once

#include <camera.h>
#include <mesh.h>

#define SUCCESS 0
#define MALLOC_FAILURE 1

typedef struct player_t {
    Mesh_t * mesh;
    Camera_t* camera;

    mat4 view;
    mat4 model;
    mat4 projection;
} Player_t;

Player_t * player_init(Camera_t * camera, Mesh_t * mesh, int * error);
vec3* player_getPositon(Player_t * player);
void player_setView(Player_t * player, mat4 view);
void player_setModel(Player_t * player, mat4 model);
void player_setProjection(Player_t * player, mat4 projection);
void player_render(Player_t * player, Shader_t * shader, int * error);