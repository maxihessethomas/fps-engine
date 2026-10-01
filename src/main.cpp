#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION

#include "../lib/stb/stb_image.h"

#include "glad.h"
#include "GLFW/glfw3.h"

#include "mesh.hpp"
#include "math.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "buffer.hpp"
#include "class.hpp"

#include "collision.hpp"

const int SCREEN_W = 800;
const int SCREEN_H = 600;
const float SENSITIVITY = 0.002f;
const float GRAVITY = 0.981f;

std::string readFile(const char* path) {
    std::ifstream file(path);
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

    Rectangle hitbox = {camera.position, 1, 2};

    glfwSetWindowUserPointer(window, &camera);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, Camera::mouseCallback);

// Vertex layout: position (3), UV (2), normal (3)
float cubeVertices[] = {
    // Front face (+Z)
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,

    // Back face (-Z)
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,

    // Left face (-X)
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,

    // Right face (+X)
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,

    // Top face (+Y)
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,

    // Bottom face (-Y)
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f
};

unsigned int cubeIndices[] = {
    // Front
     0,  1,  2,   2,  3,  0,

    // Back
     4,  5,  6,   6,  7,  4,

    // Left
     8,  9, 10,  10, 11,  8,

    // Right
    12, 13, 14,  14, 15, 12,

    // Top
    16, 17, 18,  18, 19, 16,

    // Bottom
    20, 21, 22,  22, 23, 20
};

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

    Rectangle collisionBox = {{0.0f, 0.0f, 0.0f}, 10, 1};

    std::string vertexShaderSource = readFile("src/basic.vert");
    std::string fragmentShaderSource = readFile("src/basic.frag");
    Shader shader(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

    Mesh mesh(vertices, sizeof(vertices), indices, sizeof(indices), (sizeof(indices) / sizeof(int)));
    Mesh cubeMesh(cubeVertices, sizeof(cubeVertices), cubeIndices, sizeof(cubeIndices), 36);

    Texture grid_texture("textures/grid.png");

    const char* skybox[] = {
        "textures/skybox/right.jpg",
        "textures/skybox/left.jpg",
        "textures/skybox/top.jpg",
        "textures/skybox/bottom.jpg",
        "textures/skybox/front.jpg",
        "textures/skybox/back.jpg"
    };

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    std::string skyboxVertexShader = readFile("src/skybox.vert");
    std::string skyboxFragmentShader = readFile("src/skybox.frag");
    Shader skyboxShader(skyboxVertexShader.c_str(), skyboxFragmentShader.c_str());

    VertexArray skyboxVAO;
    VertexBuffer skyboxVBO(skyboxVertices, sizeof(skyboxVertices));

    skyboxVAO.bind();
    skyboxVBO.bind();

    skyboxVAO.setAttribute(0, 3, 3 * sizeof(float), 0);

    skyboxVAO.unbind();

    CubeMap cubemap(skybox);

    Light light = {
        {0, 10, 0},
        {0.1f, 0.1f, 0.1f},
        {0.8f, 0.8f, 0.8f},
        {1.0f, 1.0f, 1.0f}
    };

    double initTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
  /*       printf("%f, %f, %f \n", hitbox.position.x, hitbox.position.y, hitbox.position.z); */

        int collision = AABB(collisionBox, hitbox);
        if (collision == 1) {
            std::cout << "collision\n";
        }
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - initTime);
        initTime = currentTime;

        camera.keyInput(window, deltaTime);
        hitbox.position = camera.position;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthFunc(GL_LEQUAL);

        skyboxShader.use();
        skyboxShader.mat4Set("projection", camera.perspective(camera.fov, aspect(SCREEN_W, SCREEN_H), camera.near, camera.far));
        skyboxShader.mat4Set("view", camera.skyboxView());

        glActiveTexture(GL_TEXTURE0);
        cubemap.bind();
        skyboxShader.intSet("skybox", 0);

        skyboxVAO.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        skyboxVAO.unbind();

        glDepthFunc(GL_LESS);

        shader.use();
        shader.mat4Set("view", camera.view());
        shader.mat4Set("projection", camera.perspective(camera.fov, aspect(SCREEN_W, SCREEN_H), camera.near, camera.far));
        shader.mat4Set("model", mat4Identity());
        shader.vec3Set("viewPos", camera.position);
        shader.vec3Set("material.ambient", {0.0f, 0.0f, 0.0f});
        shader.vec3Set("material.diffuse", {1.0f, 1.0f, 1.0f});
        shader.vec3Set("material.specular", {0.5f, 0.5f, 0.5f});
        shader.floatSet("material.shininess", 32.0f);
        shader.vec3Set("light.position", light.position);
        shader.vec3Set("light.ambient",  light.ambient);
        shader.vec3Set("light.diffuse",  light.diffuse);
        shader.vec3Set("light.specular", light.specular);
        
        grid_texture.bind();
        mesh.draw();
        cubeMesh.draw();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec3 texcoords;
};