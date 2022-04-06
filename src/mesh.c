#include <mesh.h>

Mesh_t *mesh_init(Vertex_t *verteces, Texture_t *textures, unsigned int *indices, unsigned int vertexCount, unsigned int textureCount, unsigned int indicesCount, int *error) {
    if(verteces == NULL) {
        *error = VERTECES_NULL;
        return NULL;
    }
    if(indices == NULL) {
        *error = INDICES_NULL;
        return NULL;
    }
    if(vertexCount==0||indicesCount==0) {
        *error = ILLEGAL_SIZE_ARGUMENT;
        return NULL;
    }
    Mesh_t * mesh = malloc(sizeof(Mesh_t));
    if(mesh==NULL) {
        *error = MESH_ALLOC_FAILURE;
        return NULL;
    }
    mesh->indicecount = indicesCount;
    mesh->texturecount = textureCount;
    mesh->vertexcount = vertexCount;

    mesh->verteces = verteces;
    mesh->indices = indices;
    mesh->textures = textures;

    mesh_setupMesh(mesh);
    *error = SUCCESS;
    return mesh;
}

void mesh_setupMesh(Mesh_t *mesh) {
    // Get a free reference id for the buffer
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &(mesh->VBO));
    glGenBuffers(1, &(mesh->EBO));

    glBindVertexArray(mesh->VAO);
    // load data into vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertexcount * sizeof(Vertex_t), mesh->verteces, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicecount * sizeof(unsigned int), mesh->indices, GL_STATIC_DRAW);

    // Setup the vertex attribute pointers:
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void *)offsetof(Vertex_t, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void *)offsetof(Vertex_t, texCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void *)offsetof(Vertex_t, tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void *)offsetof(Vertex_t, bitangent));

    glBindVertexArray(0);
}

void mesh_draw(Mesh_t * mesh, Shader_t * shader, int * error) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    #define LEN 5
    char number[LEN];
    for(unsigned int i = 0; i < mesh->texturecount; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        const char * name = mesh->textures[i].type;
        if(!strcmp("texture_diffuse", name)) {
            snprintf(number, LEN, "%d", diffuseNr++);
        }else if(!strcmp("texture_specular",name)) {
            snprintf(number, LEN, "%d", specularNr++);
        } else if(!strcmp("texture_normal", name)) {
            snprintf(number, LEN, "%d", normalNr++);
        } else if(!strcmp("texture_height", name)) {
            snprintf(number, LEN, "%d", heightNr++);
        }

        const size_t namelen = strlen(name);
        const size_t numberlen = strlen(number);
        char * str = malloc(namelen + numberlen + 1);
        if(str == NULL) {
            *error = STRING_ALLOC_FAILURE;
            return;
        }
        // NOt the fastest way, but it does the job
        memcpy(str, name, namelen);
        memcpy(str + namelen, number, numberlen + 1);
        glUniform1i(glGetUniformLocation(shader->id, str), i);
        glBindTexture(GL_TEXTURE_2D, (mesh->textures[i]).id);
        free(str);
#undef LEN
    }
    // Render the stuff
    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->indicecount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    // Reset 
    glActiveTexture(GL_TEXTURE0);
    *error = SUCCESS;
}

Vertex_t* vertex_init(vec3 pos, vec3 normal, vec2 texCoords, vec3 tangent, vec3 bitangent) {
    Vertex_t * buffer = (struct vertex_t*) malloc(sizeof(struct vertex_t));
    if(buffer == NULL) {
        return NULL;
    }
    memcpy(buffer->pos, pos, sizeof(vec3));
    memcpy(buffer->normal, normal, sizeof(vec3));
    memcpy(buffer->texCoords, texCoords, sizeof(vec3));
    memcpy(buffer->tangent, tangent, sizeof(vec3));
    memcpy(buffer->bitangent, bitangent, sizeof(vec3));

    return buffer;
}

// Initialize empty vertex
Vertex_t* vertex_inite() {
    return (struct vertex_t*) calloc(1, sizeof(struct vertex_t));
}


int vertex_addPos(Vertex_t * vertex, vec3 pos) {
    if(vertex==NULL) {
        return VERTEX_IS_NULL_ERROR;
    } else {
        memcpy(vertex->pos, pos, sizeof(vec3));
        return SUCCESS;
    }
}


int vertex_addNormal(Vertex_t * vertex, vec3 normal) {
    if(vertex==NULL) {
        return VERTEX_IS_NULL_ERROR;
    } else {
        memcpy(vertex->normal, normal, sizeof(vec3));
        return SUCCESS;
    }
}

int vertex_addTexCoords(Vertex_t * vertex, vec2 texCoords) {
    if(vertex==NULL) {
        return VERTEX_IS_NULL_ERROR;
    } else {
        memcpy(vertex->texCoords, texCoords, sizeof(vec3));
        return SUCCESS;
    }
}