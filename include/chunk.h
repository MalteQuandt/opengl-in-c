#include <stdint.h>
#include <cglm/cglm.h>

#define CHUNK_WIDTH 32
#define CHUNK_HEIGHT 32

// A single element in a static chunk
typedef struct tile_t {
    // The color data of the face
    vec4 color;
} Tile_t;

// A single chunk
typedef struct chunkt_t {
    // Uniquie id for the chunk:
    unsigned id;
    // World position
    vec3 position;
    float height;
    float width;
    // Element array for the current chunk
    Tile_t elem[CHUNK_HEIGHT][CHUNK_WIDTH];
} Chunk_t;