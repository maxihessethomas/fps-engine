#include <string>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION

#include "../lib/stb/stb_image.h"

#include "glad.h"
#include "GLFW/glfw3.h"

#include "import.hpp"
#include "mesh.hpp"
#include "math.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "buffer.hpp"
#include "class.hpp"

#include "collision.hpp"

const int SCREEN_W = 1800;
const int SCREEN_H = 1000;
const float SENSITIVITY = 0.002f;
const float GRAVITY = 9.81f;
bool WIREFRAME = false;

int main() {
    if (!glfwInit() ){
        printf("INFO: GLFW FAILED");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "fps", NULL, NULL);
    if (!window) {
        printf("INFO: window creation failed");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("INFO: GLAD failed");
        glfwTerminate();
    }

    glEnable(GL_DEPTH_TEST);

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

/*     Mesh mesh(vertices, sizeof(vertices), indices, sizeof(indices), (sizeof(indices) / sizeof(int)));
    Texture grid_texture("textures/grid.png"); */

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

/*     std::string skyboxVertexShader = readShader("src/skybox.vert");
    std::string skyboxFragmentShader = readShader("src/skybox.frag");
    Shader skyboxShader(skyboxVertexShader.c_str(), skyboxFragmentShader.c_str()); */

/*     VAO skyboxVAO;
    VBO skyboxVBO;
    skyboxVBO.init(skyboxVertices, sizeof(skyboxVertices));

    skyboxVAO.bind();
    skyboxVBO.bind();

    skyboxVAO.setAttribute(0, 3, 3 * sizeof(float), 0, GL_FLOAT);

    skyboxVAO.unbind();

    CubeMap cubemap(skybox);
 */
    Light light = {
        {0, 10, 0},
        {0.1f, 0.1f, 0.1f},
        {0.8f, 0.8f, 0.8f},
        {1.0f, 1.0f, 1.0f}
    };

    double initTime = glfwGetTime();

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    readOBJ("assets/level1.obj", vertices, indices);

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - initTime);
        initTime = currentTime;

        camera.keyInput(window, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            WIREFRAME = !WIREFRAME;
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthFunc(GL_LEQUAL);

/*         skyboxShader.use();
        skyboxShader.mat4Set("projection", camera.perspective(camera.fov, aspect(SCREEN_W, SCREEN_H), camera.near, camera.far));
        skyboxShader.mat4Set("view", camera.skyboxView());

        glActiveTexture(GL_TEXTURE0);
        cubemap.bind();
        skyboxShader.intSet("skybox", 0);

        skyboxVAO.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        skyboxVAO.unbind(); */

        glDepthFunc(GL_LESS);

/*         shader.use();
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
        mesh.draw(); */

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    GLFWwindow* debug = glfwCreateWindow(600, 600, "debug", NULL, NULL);
    if (!debug) {
        glfwTerminate();
        return -1;
    }

    glfwDestroyWindow(debug);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

