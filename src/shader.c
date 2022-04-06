#include "shader.h"

Shader_t* shader_init(const char * vertexPath, const char * fragmentPath) {
    if(vertexPath == NULL && fragmentPath == NULL) {
        return NULL;
    }
    Shader_t * shader = (struct shader*) malloc(sizeof(struct shader));
    // Variable to store the error code
    int error = 0;
    // Get the shader source code:
    char * vShaderSource = loadFile(vertexPath, &error);
    char * fShaderSource = loadFile(fragmentPath, &error);
    // Generate the shader:
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

// Generate a new shader program and then return the reference value to it
GLint generate_shader(const char * vShaderSource,const char * fShaderSource)
{
    GLint success;
    char infoLog[512];
    // Declare references for the shaders (vertex and fragment)
    unsigned int fragmentShader, vertexShader, shaderProgram;

    // Generate the vertex buffer object for the color points:
    // Generate the vertex shader object
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Generate the fragment shader object
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Attach the source code to the shader objects:
    glShaderSource(vertexShader, 1, &vShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &fShaderSource, NULL);

    // Compile the shaders:
    glCompileShader(vertexShader);
    // some simple error checking:
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FALIED\n");
		printf("%s\n", infoLog);
        return -1;
    }
    // Repeat the last two actions (compiling and error checking) again for the fragment shader
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FALIED\n");
		printf("%s\n", infoLog);
        return -1;
    }
    // Create the shader program object
    shaderProgram = glCreateProgram();
    // attach the shaders to the program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Tell the linker which attribute should have which location (in case we can't use the location is
    // glsl like in opengl 3.2)
    // glBindAttribLocation(shaderProgram, 0, "pos");
    // glBindAttribLocation(shaderProgram, 1, "col");
    // Link the shaders together for the final program
    glLinkProgram(shaderProgram);
    // Do some error handling if the linking was successful
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::LINKING");
		printf("%s\n", infoLog);
        return -1;
    }
    // The shaders are now linked together in the final program and are therefore
    // no longer needed, we can now delete the shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // Activate the shader program, so it will be used when rendering objects
    glUseProgram(shaderProgram);
    return shaderProgram;
}