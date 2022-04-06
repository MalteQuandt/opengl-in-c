#include <player.h>

Player_t * player_init(Camera_t * camera, Mesh_t * mesh, int * error) {
    Player_t * buffer = malloc(sizeof(Player_t));
    if(buffer==NULL){
        *error = MALLOC_FAILURE;
        return NULL;
    } else {
        buffer->camera = camera;
        buffer->mesh = mesh;
        *error = SUCCESS;
    }
    return buffer;
}

void player_setView(Player_t * player, mat4 view) {
    memcpy(player->view, view, sizeof(mat4));
}
vec3* player_getPositon(Player_t * player) {
    return player->camera->pos;
}
void player_setModel(Player_t * player, mat4 model) {
    memcpy(player->model, model, sizeof(mat4));
}
void player_setProjection(Player_t * player, mat4 projection) {
    memcpy(player->projection, projection, sizeof(mat4));
}

void player_render(Player_t * player, Shader_t * shader, int * error) {
    // Generate the shader:
    glm_mat4_identity(player->model);
    glm_translate(player->model, (vec3){player->camera->pos[0], player->camera->pos[1], 0.0f});
    glm_scale(player->model, (vec3) {0.5f,1.0f,1.0f});

    // Send the data to the shader:
    shader_setMat4(shader, "projection", player->projection);
    shader_setMat4(shader, "view", player->view);
    shader_setMat4(shader, "model", player->model);

    // Draw the model:
    shader_use(shader);
    mesh_draw(player->mesh, shader, error);
}