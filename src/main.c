#include <stdio.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// User defined:
#include <shader.h>
#include <mesh.h>
#include <camera.h>
#include <player.h>

// Size of the window
#define SCR_WIDTH 800
#define SCR_HEIGHT 600

// Current state of the application
static struct state {
    unsigned int windowWidth;
    unsigned int windowHeight;

    float lastX;
    float lastY;

    int firstMouse;
    float deltaTime;
    float lastFrame;
    
    int error;
    Camera_t * camera;
} state;

// Callback functions:
void framebuffer_size_callback(GLFWwindow *, int, int);
void mouse_callback(GLFWwindow*, double, double);
void scroll_callback(GLFWwindow*, double, double);
void processInput(GLFWwindow*);

int main()
{
    // Initialize the global state
    state.firstMouse = 1;
    state.lastX = SCR_WIDTH / 2.0f;
    state.lastY = SCR_HEIGHT / 2.0f;

    state.deltaTime = 0.0f;
    state.lastFrame = 0.0f;

    state.error = 0;
    // Initialize glfw
    glfwInit();
    // Give GLFW the hint at what version we want to be using:
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Make sure we only get access to core profile functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Generate a new window with the dimensions 800 by 800 and the title GL
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GL", NULL, NULL);
    // some error checking if the window creation failed
    if (window == NULL)
    {
        // If the creation failed, abort the program and return the glfw resources
        // with the glfwterminate method.
        printf("Failed to create GLFW window");
        glfwTerminate();
        // Return an error code.
        return -1;
    }
    // Make the context of the current window current
    glfwMakeContextCurrent(window);

    // Initialzize glad:
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return -1;
    }
    // Set the callback function for the case that the user resizes the window:
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Vertex_t mesh1[] = {
        {.pos={0.5f,  0.5f, 0.0f}, .normal={0.0f, 0.0f, 1.0f}, .texCoords={1.0f, 1.0f}, .tangent={0.0f, 0.0f, 0.0f}, .bitangent={0.0f, 0.0f, 0.0f}},
        {.pos={0.5f, -0.5f, 0.0f}, .normal={0.0f, 0.0f, 1.0f}, .texCoords={1.0f, 0.0f}, .tangent={0.0f, 0.0f, 0.0f}, .bitangent={0.0f, 0.0f, 0.0f}},
        {.pos={-0.5f, -0.5f, 0.0f},.normal={0.0f, 0.0f, 1.0f}, .texCoords={0.0f, 0.0f}, .tangent={0.0f, 0.0f, 0.0f}, .bitangent={0.0f, 0.0f, 0.0f}},
        {.pos={-0.5f,  0.5f, 0.0f}, .normal={0.0f, 0.0f, 1.0f}, .texCoords={0.0f, 1.0f}, .tangent={0.0f, 0.0f, 0.0f}, .bitangent={0.0f, 0.0f, 0.0f}}
    };
    vec3 pos[] = {
    {0.0f, -2.0f, 0.0f},
    {1.0f, -2.0f, 0.0f},
    {2.0f, -2.0f, 0.0f},
    {3.0f, -2.0f, 0.0f},};
    printf("Create the camera...\n");
    state.camera = camera_init(
        (vec3){0.0f, 0.0f, 5.0f},
        (vec3){0.0f, 1.0f, 0.0f},
        YAW, PITCH, (int*){0});
    camera_setFlags(state.camera, DISABLE_ZOOM | DISABLE_MOUSE_ROTATION);
    printf("Load the shaders...\n");
    // Generate the rendering/shader program
    Shader_t * shader = shader_init("shaders\\vertexShader01.vs", "shaders\\fragmentShader01.fs");
    unsigned int indices[6] = {0,1,3, 1,2,3};
    Mesh_t * mesh = mesh_init(&mesh1[0], NULL, indices, 4, 0, 6, &state.error);
    // Generate the player information:
    Mesh_t * pmesh = mesh_init(mesh1, NULL, indices, 4, 0, 6, &state.error);
    Player_t * player = player_init(state.camera, pmesh, &state.error);
    // Set the clear color:
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glDepthFunc(GL_LESS);

    state.windowWidth = SCR_WIDTH;
    state.windowHeight = SCR_HEIGHT;
    printf("Start the rendering...\n");
    // Tell opengl to draw in wire frame mode:
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = glfwGetTime();
        state.deltaTime = currentFrame - state.lastFrame;
        state.lastFrame = currentFrame;

        // Process the input
        processInput(window);

        // Reset the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        // Set the shader program "shader" to use for rendering
        shader_use(shader);
        // Create the transformation pipeline and send it to the gpu:
        mat4 projection;
        mat4 view;
        mat4 model;
        // Calculate the transformation pipeline
        
        glm_perspective(glm_rad(state.camera->zoom), (float) state.windowWidth / (float) state.windowHeight, 0.1f, 100.0f, projection);
        camera_getViewMat(state.camera, view);

        // Send the transformation matrices to the shader
        shader_setMat4(shader, "projection", projection);
        shader_setMat4(shader, "view", view);
        for(long long unsigned int i = 0; i < sizeof(pos)/sizeof(vec3); i++) {
            // Generate the model matrix
            glm_mat4_identity(model);
            glm_translate(model,pos[i]);
            glm_scale(model, (vec3) {2.0f, 2.0f, 2.0f});
            // Send the new model to the gpu
            shader_setMat4(shader, "model", model);
            // Draw the mesh:
            mesh_draw(mesh, shader, &state.error);
        }
        // Send transformation pipeline to player
        player_setView(player, view);
        player_setProjection(player, projection);
        player_setModel(player, model);
        
        // Render the player:
        player_render(player, shader, &state.error);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// Utility functions:
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, 1);
        printf("The window will now close...\n");
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera_processKeyboard(state.camera, FORWARD, state.deltaTime);}
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera_processKeyboard(state.camera, BACKWARD, state.deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera_processKeyboard(state.camera, LEFT, state.deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera_processKeyboard(state.camera, RIGHT, state.deltaTime);}
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera_processKeyboard(state.camera, UP, state.deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera_processKeyboard(state.camera, DOWN, state.deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        // Print the current state of the engine
        printf("Yaw and Pitch: ");
        printf("(%f;%f)\n", state.camera->yaw, state.camera->pitch);
        printf("Position: \n");
        printf("(%f;%f;%f)\n", state.camera->pos[0], state.camera->pos[1], state.camera->pos[2]);
    }
}
void framebuffer_size_callback(__attribute__((unused)) GLFWwindow *window, int width, int height)
{
    state.windowWidth = width;
    state.windowHeight = height;   
    // Set the new viewport dimensions after the resize:
    glViewport(0, 0, width, height);
}

void scroll_callback(__attribute__((unused)) GLFWwindow* window,__attribute__((unused)) double xoffset,__attribute__((unused)) double yoffset) {
    // camera_processMouseScroll(camera, yoffset);
}

void mouse_callback(__attribute__((unused)) GLFWwindow* window, double xpos, double ypos) {
    if(state.firstMouse) {
        state.lastX = xpos;
        state.lastY = ypos;
        state.firstMouse = 0;
    }
    __attribute__((unused)) float xoffset = xpos - state.lastX;
    __attribute__((unused)) float yoffset = state.lastY - ypos;

    state.lastX = xpos;
    state.lastY = ypos;

    //camera_processMouseMovement(camera, xoffset, yoffset, 1);
}