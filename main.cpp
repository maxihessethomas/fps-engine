#include "glad.h"
#include "lib/glfw/include/GLFW/glfw3.h"
#include "class.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

const int SCREEN_W = 800;
const int SCREEN_H = 600;
const float SENSITIVITY = 0.002f;
const float GRAVITY = 0.981f;

std::string readFile(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Failed to load file";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLFWwindow* GLFWInit(int width, int height, const char* name) {
    if(!glfwInit()) {
        std::cout << "INFO: GLFW failed\n";
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!window) {
        std::cout << "INFO: window creation failed\n";
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "INFO: GLAD failed\n";
        glfwTerminate();
        return NULL;
    }

    glEnable(GL_DEPTH_TEST);

    return window;
}

int main() {
    GLFWwindow* window = GLFWInit(SCREEN_W, SCREEN_H, "fps");
    if (!window) return -1;
    
    Camera camera = {
        {0, 2, 3},
        0.0f,
        0.0f,
        0.0f,
        degreesToRadians(60),
        0.1f,
        100.0f
    };

    glfwSetWindowUserPointer(window, &camera);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, Camera::mouseCallback);


/*     std::cout << "vec1: " << vec1.x << " " << vec1.y << " " << vec1.z << "\n";
    std::cout << "vec2: " << vec2.x << " " << vec2.y << " " << vec2.z << "\n";
    std::cout << "vec3: " << vec3.x << " " << vec3.y << " " << vec3.z << "\n"; */


    float vertices[] = {
        // x,     y,    z,     u,    v,    nx,   ny,   nz
        -5.0f, 0.0f, -5.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
        5.0f, 0.0f, -5.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        5.0f, 0.0f,  5.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -5.0f, 0.0f,  5.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2, 
        2, 3, 0
    };

    std::string vertexShaderSource = readFile("basic.vert");
    std::string fragmentShaderSource = readFile("basic.frag");
    Shader shader(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

    Mesh mesh(vertices, sizeof(vertices), indices, sizeof(indices), (sizeof(indices) / sizeof(int)));

    Texture grid_texture("textures/grid.png");

    double initTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        camera.print();
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - initTime);
        initTime = currentTime;
/*         std::cout << 1 / deltaTime << "\n"; */
        camera.keyInput(window, deltaTime);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.mat4Set("view", camera.view());
        shader.mat4Set(
            "projection",
            camera.perspective(
                camera.fov,
                aspect(SCREEN_W, SCREEN_H),
                camera.near,
                camera.far
            )
        );
        shader.mat4Set("model", mat4Identity());
        shader.vec3Set("objectColor", {1.0, 0.5, 0.31});
        shader.vec3Set("viewPos", camera.position);
        shader.vec3Set("material.ambient", {1.0f, 0.5f, 0.31f});
        shader.vec3Set("material.diffuse", {1.0f, 0.5f, 0.31f});
        shader.vec3Set("material.specular", {0.5f, 0.5f, 0.5f});
        shader.floatSet("material.shininess", 32.0f);
        shader.vec3Set("light.position", {5.0f, 5.0f, 0.0f});
        shader.vec3Set("light.ambient",  {0.2f, 0.2f, 0.2f});
        shader.vec3Set("light.diffuse",  {0.5f, 0.5f, 0.5f}); // darken diffuse light a bit
        shader.vec3Set("light.specular", {1.0f, 1.0f, 1.0f}); 
        
        grid_texture.bind();
        mesh.draw();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}