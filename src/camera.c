#include <camera.h>

Camera_t * camera_init(vec3 pos, vec3 up, float yaw, float pitch, int * error) {
    Camera_t * buffer = (struct camera_t*) malloc(sizeof(Camera_t));
    if(buffer == NULL) {
        *error = BUFFER_ALLOCATION_FAILURE;
        return NULL;
    }
    memcpy(buffer->pos, pos, sizeof(vec3));
    memcpy(buffer->worldUp, up, sizeof(vec3));
    buffer->yaw = yaw;
    buffer->pitch = pitch;
    buffer->flags = 0;
    // Constant values 
    buffer->movementspeed = SPEED;
    buffer->mousesensitivity = SENSITIVITY;
    buffer->zoom = ZOOM;
    camera_updateCameraVectors(buffer);
    return buffer;
}

Camera_t * camera_inite(int * error) {
    Camera_t * buffer = (struct camera_t*) malloc(sizeof(Camera_t));
    if(buffer == NULL) {
        *error = BUFFER_ALLOCATION_FAILURE;
        return NULL;
    }
    memcpy(buffer->pos, (vec3){0.0f, 0.0f, 0.0f}, sizeof(vec3));
    memcpy(buffer->up, (vec3){0.0f, 1.0f, 0.0f}, sizeof(vec3));
    buffer->yaw = YAW;
    buffer->pitch = PITCH;
    buffer->flags = 0;
    // Constant values 
    buffer->movementspeed = SPEED;
    buffer->mousesensitivity = SENSITIVITY;
    buffer->zoom = ZOOM;
    camera_updateCameraVectors(buffer);
    return buffer;
}

void camera_getViewMat(Camera_t * camera, mat4 dest) {
    vec3 center;
    glm_vec3_add(camera->pos, camera->front, center);
    glm_lookat(camera->pos, center, camera->up, dest);
}

void camera_processMouseScroll(Camera_t * camera, float yoffset) {
    camera->zoom -= (float) yoffset;
    if(camera->zoom < 1.0f) {
       camera->zoom = 1.0f; 
    }
    if(camera->zoom > 90.0f) {
        camera->zoom = 90.0f;
    }
}

void camera_updateCameraVectors(Camera_t * camera) {
    if(camera == NULL) {
        return;
    }else {
        // Calculate the new front vector
        vec3 front;
        front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
        front[1] = sin(glm_rad(camera->pitch));
        front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
        glm_normalize(front);
        memcpy(camera->front, front, sizeof(vec3));

        // Create the right direction vector
        vec3 cross;
        glm_cross(camera->front, camera->worldUp, cross);
        glm_normalize(cross);
        memcpy(camera->right, cross, sizeof(vec3));

        // Create the up direction vector:
        glm_cross(camera->right, camera->front, cross);
        glm_normalize(cross);
        memcpy(camera->up, cross, sizeof(vec3));
        return;
    }
}

void camera_processMouseMovement(Camera_t * camera, float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= camera->mousesensitivity;
    yoffset *= camera->mousesensitivity;

    camera->yaw += xoffset;
    camera->pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;
    }
    camera_updateCameraVectors(camera);
}
// Process the keyboard but don't take the flags into account
void camera_processKeyboard(Camera_t * camera, Cameramov mov,float deltaTime) {
    float velo = camera->movementspeed * deltaTime;
    vec3 cross, worldFront;
    glm_cross(camera->worldUp, camera->right, cross);
    glm_normalize(cross);
    memcpy(worldFront, cross, sizeof(vec3));
    vec3 mult, add;
    switch(mov) {
        case FORWARD:
        glm_vec3_scale(worldFront, velo, mult);
        glm_vec3_add(camera->pos, mult, add);
        break;
        case BACKWARD:
        glm_vec3_scale(worldFront, velo, mult);
        glm_vec3_sub(camera->pos, mult, add);
        break;
        case LEFT:
        glm_vec3_scale(camera->right, velo, mult);
        glm_vec3_sub(camera->pos, mult, add);
        break;
        case RIGHT:
        glm_vec3_scale(camera->right, velo, mult);
        glm_vec3_add(camera->pos, mult, add);
        break;
        case UP:
        glm_vec3_scale(camera->worldUp, velo, mult);
        glm_vec3_add(camera->pos, mult, add);
        break;
        case DOWN:
        glm_vec3_scale(camera->worldUp, velo, mult);
        glm_vec3_sub(camera->pos, mult, add);
        break;
        default:
            fprintf(stderr, "This direction is not supported!");
            break;
    }
    // copy the resulting position vector into the camera
    memcpy(camera->pos, add, sizeof(vec3));
    return;
}

void camera_setFlags(Camera_t * camera, uint32_t flags) {
    camera->flags = flags;
}