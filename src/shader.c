#include "shader.h"

Shader_t* shader_init(const char * vertexPath, const char * fragmentPath) {
    if(vertexPath == NULL && fragmentPath == NULL) {
        return NULL;
    }
    Shader_t * shader = (struct shader*) malloc(sizeof(struct shader));
    int error = 0;
    char * vShaderSource = loadFile(vertexPath, &error);
    char * fShaderSource = loadFile(fragmentPath, &error);
    shader->id = generate_shader(vShaderSource, fShaderSource);
    free(vShaderSource);
    free(fShaderSource);
    return shader;
}
void shader_use(Shader_t * shader) {
    glUseProgram(shader->id);
}

inline void shader_setBool(Shader_t * restrict shader, const char * name, int value) {
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}
inline void shader_setInt(Shader_t * restrict shader, const char * name, int value) {
    shader_setBool(shader, name, value);
}

inline void shader_setFloat(Shader_t * restrict shader, const char * name, float value) {
    glUniform1f(glGetUniformLocation(shader->id, name), value);
}

inline void shader_setMat4(Shader_t * restrict shader, const char * name, mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(shader->id, name),1,GL_FALSE, (float*) value);
}

inline void shader_setVec3(Shader_t * restrict shader, const char * name, vec3 value) {
    glUniform3fv(glGetUniformLocation(shader->id, name), 1, (float*)value);
}

inline void shader_setVec3xyz(Shader_t * restrict shader, const char * name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(shader->id, name)  , x, y, z);
}

GLint generate_shader(const char * vShaderSource,const char * fShaderSource)
{
    GLint success;
    char infoLog[512];
    unsigned int fragmentShader, vertexShader, shaderProgram;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &fShaderSource, NULL);

    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FALIED\n");
		printf("%s\n", infoLog);
        return -1;
    }
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FALIED\n");
		printf("%s\n", infoLog);
        return -1;
    }
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::LINKING");
		printf("%s\n", infoLog);
        return -1;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(shaderProgram);
    return shaderProgram;
}