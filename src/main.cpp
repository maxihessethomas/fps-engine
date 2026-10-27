#include "include/PrecompiledHeaders.h"

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

    Skybox skybox("shaders/skybox.vert", "shaders/skybox.frag", skyboxPath);

    Light light = {
        {0, 5, 0},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f}
    };

    double initTime = glfwGetTime();

    Mesh shotgun("shaders/lighting.vert", "shaders/lighting.frag", "assets/c_shotgun.obj", "textures/c_shotgun.png", light, camera, SCREEN_W, SCREEN_H);
    Mesh level("shaders/lighting.vert", "shaders/lighting.frag", "assets/plane.obj", "textures/grid.png", light, camera, SCREEN_W, SCREEN_H);
    Mesh cube("shaders/lighting.vert", "shaders/lighting.frag", "assets/cube.obj", "textures/grid.png", light, camera, SCREEN_W, SCREEN_H);
  /*   Mesh pill("shaders/lighting.vert", "shaders/lighting.frag", "assets/pill.obj", "textures/grid.png", light, camera, SCREEN_W, SCREEN_H);
 */
    static float cubeRotationX = 0.0f;
    static float cubeRotationZ = 0.0f;
    Vec3 shotgunOffset = {0.7f, -0.4f, -0.8f};
    Vec3 levelOffset = {0.0f, 0.0f, 0.0f};

    bool shotgundraw = false;
    bool sniperdraw = false;
    bool gravity = false;
    bool windowShouldClose = false;
    bool backFaceCulling = false;
    bool wireframeMode = false;

/*     Vec2 data = {0.0f, 0.0f};
    Vec2* pointer = &data;
    printf("%f", sizeof(pointer)); */

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration;

/*     unsigned int depthFBO;
    glGenFramebuffers(1, &depthFBO);
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  */

/*     unsigned int heightFBO, heightTexture;
    glGenFramebuffers(1, &heightFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, heightFBO);
    glGenTextures(1, &heightTexture);
    glBindTexture(GL_TEXTURE_2D, heightTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, heightTexture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); */

    while (!window.shouldClose()) {

        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - initTime);
        initTime = currentTime;

        camera.keyInput(window.window, deltaTime);
        if (gravity) {
            camera.gravity(GRAVITY, deltaTime);
        }
        if (glfwGetKey(window.window, GLFW_KEY_1) == GLFW_PRESS) {
            WIREFRAME = !WIREFRAME;
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skybox.draw(camera, SCREEN_W, SCREEN_H);
        level.draw(camera, light, SCREEN_W, SCREEN_H, SCENE, mat4Translate(levelOffset));
/*         pill.draw(camera, light, SCREEN_W, SCREEN_H, SCENE, mat4Identity());
 */        if (backFaceCulling) {
            window.enableBackFaceCulling();
            cube.draw(camera, light, SCREEN_W, SCREEN_H, SCENE, mat4Mul(mat4RotateX(cubeRotationX), mat4RotateZ(cubeRotationZ)));
            glEnable(GL_DEPTH_TEST);
        }
        if (shotgundraw) {
            shotgun.draw(camera, light, SCREEN_W, SCREEN_H, CAMERA, mat4Mul(mat4Mul(mat4Translate(shotgunOffset), mat4Scale(0.1f)), mat4RotateY(180.0f)));
        }
        if (camera.position.y - 2 <= -5) {
            camera.velocityY = 0;
            camera.position.y = -5 + 2;
        } 

        if (window.debugVisible) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::ShowDemoWindow();

            ImGui::Begin("Weapons");
            ImGui::SliderFloat("Shotgun X", &shotgunOffset.x, 0.0f, 10.0f, "%.1f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("Shotgun Y", &shotgunOffset.y, 0.0f, -10.0f, "%.1f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("Shotgun Z", &shotgunOffset.z, 0.0f, -10.0f, "%.1f", ImGuiSliderFlags_NoInput);
            ImGui::Checkbox("Shotgun", &shotgundraw);
            ImGui::End();

            ImGui::Begin("Level");
            ImGui::SliderFloat("Level X", &levelOffset.x, 0.0f, 10.0f, "%.1f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("Level Y", &levelOffset.y, 0.0f, -10.0f, "%.1f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("Level Z", &levelOffset.z, 0.0f, -10.0f, "%.1f", ImGuiSliderFlags_NoInput);
            ImGui::End();

            ImGui::Begin("Cube");
            ImGui::SliderFloat("RotationX", &cubeRotationX, 0.0f, 90.0f, "%.1f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("RotationZ", &cubeRotationZ, 0.0f, 90.0f, "%.1f", ImGuiSliderFlags_NoInput);
            ImGui::Checkbox("Backface Culling", &backFaceCulling);
            ImGui::Checkbox("Wireframe mode", &wireframeMode);
            ImGui::End();

            ImGui::Begin("Player", NULL, flags);
            ImGui::Checkbox("Quit", &windowShouldClose);
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::Text("Position: (%.1f, %.1f, %.1f)", camera.position.x, camera.position.y, camera.position.z);
            ImGui::SliderFloat("Speed", &camera.speed, 1.0f, 10.0f, "%.1f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("Far", &camera.far, 1.0f, 1000.0f);
            ImGui::Checkbox("Gravity", &gravity);
    /*         ImGui::SliderFloat("Light X", &light.position.x, -10.0f, 10.0f, "%.1f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("Light Y", &light.position.y, -10.0f, 10.0f, "%.1f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("Light Z", &light.position.z, -10.0f, 10.0f, "%.1f", ImGuiSliderFlags_NoInput); */
            ImGui::End(); 

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

/*         glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   */
        
        glfwSetWindowShouldClose(window.window, windowShouldClose);
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

