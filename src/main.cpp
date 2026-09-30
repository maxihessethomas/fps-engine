#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION

#include "../lib/stb/stb_image.h"


#include "glad.h"
#include "../lib/glfw/include/GLFW/glfw3.h"
#include "math.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex.hpp"
#include "fragment.hpp"
#include "element.hpp"
#include "mesh.hpp"

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

    glfwSetWindowUserPointer(window, &camera);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, Camera::mouseCallback);

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

    std::string vertexShaderSource = readFile("src/basic.vert");
    std::string fragmentShaderSource = readFile("src/basic.frag");
    Shader shader(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

    Mesh mesh(vertices, sizeof(vertices), indices, sizeof(indices), (sizeof(indices) / sizeof(int)));

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

    double initTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - initTime);
        initTime = currentTime;
/*         std::cout << 1 / deltaTime << "\n"; */
        camera.keyInput(window, deltaTime);

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
        shader.vec3Set("objectColor", {1.0f, 0.5f, 1.0f});
        shader.vec3Set("viewPos", camera.position);
        shader.vec3Set("material.ambient", {1.0f, 0.5f, 1.0f});
        shader.vec3Set("material.diffuse", {1.0f, 0.5f, 1.0f});
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