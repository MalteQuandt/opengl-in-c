#pragma once

#include <cglm/cglm.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <math.h>
#include <string.h>

#define YAW  -90.0f
#define PITCH 0.0f
#define SPEED 2.5f
#define SENSITIVITY  0.1f
#define ZOOM 45.0f

#define SUCCESS 0
#define BUFFER_ALLOCATION_FAILURE 1

typedef enum cameramov {
    FORWARD = 0,
    BACKWARD,
    LEFT, 
    RIGHT,
    DOWN,
    UP
} Cameramov;

enum flags {
    DISABLE_MOUSE_ROTATION = 1,
    DISABLE_ZOOM = 2,
    DISABLE_FORWARD = 4,
    DISABLE_BACKWARD = 8,
};

typedef struct camera_t {
    vec3 pos;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldUp;

    float yaw;
    float pitch;

    float movementspeed;
    float mousesensitivity;
    float zoom;

    uint32_t flags;
} Camera_t;

// Initialize the camera with the vectors given in the arguments
Camera_t * camera_init(vec3 pos, vec3 up, float yaw, float pitch, int * error);
// Initialize the camera with default values:
Camera_t * camera_inite(int * error);

void camera_getViewMat(Camera_t* camera, mat4 dest);
void camera_setFlags(Camera_t * camera, uint32_t flags);
void camera_processKeyboard(Camera_t * camera, Cameramov mov, float deltaTime);
void camera_processMouseMovement(Camera_t * camera, float xoffset, float yoffset, GLboolean constrainPitch);
void camera_processMouseScroll(Camera_t * camera, float yoffset);
void camera_updateCameraVectors(Camera_t * camera);