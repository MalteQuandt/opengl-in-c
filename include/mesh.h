#pragma once

#include <cglm/cglm.h>
#include <stddef.h>
#include <shader.h>

#define SUCCESS 0
#define SHADER_NULL 1
#define VERTECES_NULL 2
#define INDICES_NULL 3
#define ILLEGAL_SIZE_ARGUMENT 4
#define MESH_ALLOC_FAILURE 5
#define STRING_ALLOC_FAILURE 6
#define VERTEX_IS_NULL_ERROR 7

// Simple vertex type with all the important data
typedef struct vertex_t
{
    vec3 pos;
    vec3 normal;
    vec2 texCoords;
    vec3 tangent;
    vec3 bitangent;
} Vertex_t;

// Simple texture object
typedef struct texture_t
{
    unsigned int id;
    const char *type;
    const char *path;
} Texture_t;

// Simple mesh type
typedef struct mesh_t
{
    // Lists of the relevant data
    Vertex_t *verteces;
    unsigned int *indices;
    Texture_t *textures;

    // How long the lists are:
    size_t vertexcount;
    size_t indicecount;
    size_t texturecount;

    // Vertex array object, vertex buffer object and element buffer object references
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
} Mesh_t;

// Initialize the data of the mesh
Mesh_t *mesh_init(Vertex_t *verteces, Texture_t *textures, unsigned int *indices, unsigned int vertexCount, unsigned int textureCount, unsigned int indicesCount, int* error);
// Draw the mesh
void mesh_draw(Mesh_t *mesh, Shader_t *shader, int * error);
// Set up the mesh
void mesh_setupMesh(Mesh_t *mesh);
// Delete a mesh from memory
int mesh_destroyMesh(Mesh_t *mesh);
// Create a vertex.
Vertex_t* vertex_init(vec3 pos, vec3 normal, vec2 texCoords, vec3 tangent, vec3 bitangent);
// Initialize empty vertex:
Vertex_t* vertex_inite();
// Add position data to a vertex
int vertex_addPos(Vertex_t * vertex, vec3 pos);
// Add a normal to a vertex
int vertex_addNormal(Vertex_t * vertex, vec3 normal);
// Add texture coordinate data to a vertex
int vertex_addTexCoords(Vertex_t * vertex, vec2 texCoords);