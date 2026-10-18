#include <string>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION

#include "../lib/stb/stb_image.h"

#include "glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "import.hpp"
#include "mesh.hpp"
#include "math.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "buffer.hpp"
#include "light.hpp"
#include "skybox.h"
#include "window.h"

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

    Window window;
    window.init(SCREEN_W, SCREEN_H, "fps");

/*     GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "fps", NULL, NULL);
    if (!window) {
        printf("INFO: window creation failed");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("INFO: GLAD failed");
        glfwTerminate();
    } */

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

    glfwSetWindowUserPointer(window.window, &camera);
    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.window, Camera::mouseCallback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
  
    const char* skyboxPath[] = {
        "textures/skybox/right.jpg",
        "textures/skybox/left.jpg",
        "textures/skybox/top.jpg",
        "textures/skybox/bottom.jpg",
        "textures/skybox/front.jpg",
        "textures/skybox/back.jpg"
    };

    Skybox skybox("src/skybox.vert", "src/skybox.frag", skyboxPath);

    Light light = {
        {0, 10, 0},
        {0.1f, 0.1f, 0.1f},
        {0.8f, 0.8f, 0.8f},
        {1.0f, 1.0f, 1.0f}
    };

    double initTime = glfwGetTime();

    Mesh pistol("src/basic.vert", "src/basic.frag", "assets/pistol.obj", "textures/grid.png", light, camera, SCREEN_W, SCREEN_H);
    Mesh level("src/basic.vert", "src/basic.frag", "assets/level.obj", "textures/grid.png", light, camera, SCREEN_W, SCREEN_H);
    Mesh cube("src/basic.vert", "src/basic.frag", "assets/cube.obj", "textures/grid.png", light, camera, SCREEN_W, SCREEN_H);

    while (window.shouldClose()) {

        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - initTime);
        initTime = currentTime;

        camera.keyInput(window.window, deltaTime);
        camera.gravity(GRAVITY, deltaTime);
        if (glfwGetKey(window.window, GLFW_KEY_1) == GLFW_PRESS) {
            WIREFRAME = !WIREFRAME;
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skybox.draw(camera, SCREEN_W, SCREEN_H);
        pistol.draw(camera, light, SCREEN_W, SCREEN_H, CAMERA);
        level.draw(camera, light, SCREEN_W, SCREEN_H, SCENE);
        cube.draw(camera, light, SCREEN_W, SCREEN_H, SCENE);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Stats");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.update();
    }

/*     GLFWwindow* debug = glfwCreateWindow(600, 600, "debug", NULL, NULL);
    if (!debug) {
        glfwTerminate();
        return -1;
    }

    glfwDestroyWindow(debug);*/
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    window.destroy();
    return 0;
}

