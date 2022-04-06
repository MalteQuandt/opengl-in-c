#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <file_loading.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct shader {
    unsigned int id;
} Shader_t;

GLint generate_shader(const char * , const char * );
Shader_t* shader_init(const char * , const char * );

void shader_use(Shader_t * shader);
void shader_setBool(Shader_t * restrict , const char * , int );
void shader_setInt(Shader_t * restrict , const char * , int );
void shader_setFloat(Shader_t * restrict , const char * , float );
void shader_setMat4(Shader_t * restrict , const char * , mat4 );
void shader_setVec3(Shader_t * restrict , const char * , vec3 );
void shader_setVec3xyz(Shader_t * restrict , const char * , float , float , float );