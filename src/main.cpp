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
    Window window;
    window.init(SCREEN_W, SCREEN_H, "fps");

    Camera camera = {
        {0, 2, 3},
        0.0f,
        0.0f,
        0.0f,
        degreesToRadians(60),
        0.1f,
        100.0f
    };
    
    window.setInputMode(GLFW_CURSOR_NORMAL, camera);

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
        {0, 5, 0},
        {0.5f, 0.5f, 0.5f},
        {0.8f, 0.8f, 0.8f},
        {1.0f, 1.0f, 1.0f}
    };

    double initTime = glfwGetTime();

    Mesh pistol("src/basic.vert", "src/basic.frag", "assets/c_shotgun.obj", "textures/c_shotgun.png", light, camera, SCREEN_W, SCREEN_H);
    Mesh level("src/basic.vert", "src/basic.frag", "assets/level.obj", "textures/grid.png", light, camera, SCREEN_W, SCREEN_H);
    Mesh cube("src/basic.vert", "src/basic.frag", "assets/cube.obj", "textures/grid.png", light, camera, SCREEN_W, SCREEN_H);
    Mesh sniper("src/basic.vert", "src/basic.frag", "assets/sniper.obj", "textures/grid.png", light, camera, SCREEN_W, SCREEN_H);

    static float cubeRotationX = 0.0f;
    static float cubeRotationZ = 0.0f;
    Vec3 pistolOffset = {0.7f, -0.4f, -0.8f};

    bool pistoldraw = false;
    bool sniperdraw = false;

    while (!window.shouldClose()) {

        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - initTime);
        initTime = currentTime;

        camera.keyInput(window.window, deltaTime);
/*         camera.gravity(GRAVITY, deltaTime);
        if (glfwGetKey(window.window, GLFW_KEY_1) == GLFW_PRESS) {
            WIREFRAME = !WIREFRAME;
        } */

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skybox.draw(camera, SCREEN_W, SCREEN_H);
        level.draw(camera, light, SCREEN_W, SCREEN_H, SCENE, mat4Identity());
        cube.draw(camera, light, SCREEN_W, SCREEN_H, SCENE, mat4Mul(mat4RotateX(cubeRotationX), mat4RotateZ(cubeRotationZ)));

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Stats");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::SliderFloat("RotationX", &cubeRotationX, 0.0f, 90.0f, "%.1f", ImGuiSliderFlags_NoInput);
        ImGui::SliderFloat("RotationZ", &cubeRotationZ, 0.0f, 90.0f, "%.1f", ImGuiSliderFlags_NoInput);
        ImGui::SliderFloat("Pistol X", &pistolOffset.x, 0.0f, 10.0f, "%.1f", ImGuiSliderFlags_NoInput);
        ImGui::SliderFloat("Pistol Y", &pistolOffset.y, 0.0f, -10.0f, "%.1f", ImGuiSliderFlags_NoInput);
        ImGui::SliderFloat("Pistol Z", &pistolOffset.z, 0.0f, -10.0f, "%.1f", ImGuiSliderFlags_NoInput);
        ImGui::SliderFloat("Light X", &light.position.x, -10.0f, 10.0f, "%.1f", ImGuiSliderFlags_NoInput);
        ImGui::SliderFloat("Light Y", &light.position.y, -10.0f, 10.0f, "%.1f", ImGuiSliderFlags_NoInput);
        ImGui::SliderFloat("Light Z", &light.position.z, -10.0f, 10.0f, "%.1f", ImGuiSliderFlags_NoInput);
        ImGui::Checkbox("Shotgun", &pistoldraw);
        if (pistoldraw == true) {
            pistol.draw(camera, light, SCREEN_W, SCREEN_H, CAMERA, mat4Mul(mat4Mul(mat4Translate(pistolOffset), mat4Scale(0.1f)), mat4RotateY(180.0f)));
        }
        ImGui::Checkbox("Sniper", &sniperdraw);
        if (sniperdraw == true) {
            sniper.draw(camera, light, SCREEN_W, SCREEN_H, CAMERA, mat4Mul(mat4Mul(mat4Translate({0.7f, -0.4f, -0.8f}), mat4Scale(1.0f)), mat4RotateY(0)));
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.keyInput();
        window.updateInputMode();
        window.update();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    window.destroy();
    return 0;
}

