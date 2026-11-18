#include "EngineLN.h"

int main()
{
    EngineLN::App app;

    if (app.init())
    {
        app.run();
    }
    app.shutdown();

    return 0;
}


/* #include <PrecompiledHeaders.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Buffer/Buffer.h"
#include "Camera/Camera.h"
#include "Collision/Collision.h"
#include "Import/Import.h"
#include "Light/Light.h"
#include "Math/Math.h"
#include "Mesh/Mesh.h"
#include ""


const float SENSITIVITY = 0.002f;
const float GRAVITY = 9.81f;
bool WIREFRAME = false;
const int SCREEN_W = 800;
const int SCREEN_H = 600;


/* Notes:
* void gravity(const float gravity, const float deltaTime) {
        if (deltaTime > 0.1) {
            velocityY += gravity * 0.1;
            position.y -= velocityY * 0.1;
        } else {
            velocityY += gravity * deltaTime;
            position.y -= velocityY * deltaTime;
        };
    }


int main() {
    Window window;
    window.init(SCREEN_W, SCREEN_H, "fps");

    Player player({0, 20, 0});

    window.setInputMode(GLFW_CURSOR_NORMAL, player.camera);

    const char* skyboxPath[] = {
        "textures/skybox/right.jpg",
        "textures/skybox/left.jpg",
        "textures/skybox/top.jpg",
        "textures/skybox/bottom.jpg",
        "textures/skybox/front.jpg",
        "textures/skybox/back.jpg"
    };

    Light light = {
        {0, 5, 0},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f}
    };

    double initTime = glfwGetTime();

    float uiElement[] = {
        1.0f, 1.0f, 0.0f,
        1.0f, 0.5f, 0.0f,
        0.5f, 1.0f, 0.0f
    };

    Mesh level("shaders/lighting.vert", "shaders/lighting.frag", "assets/plane.obj", "textures/grid.png");

    bool gravity = false;
    bool windowShouldClose = false;

    while (!window.shouldClose()) {

        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - initTime);
        initTime = currentTime;

        player.camera.keyInput(window.window, deltaTime);
        if (gravity) {
            player.camera.gravity(GRAVITY, deltaTime);
        }
        if (glfwGetKey(window.window, GLFW_KEY_1) == GLFW_PRESS) {
            WIREFRAME = !WIREFRAME;
        }

        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        level.draw(player.camera, light, SCREEN_W, SCREEN_H, SCENE, mat4Identity());

        if (player.AreVerticesColliding(level)) {
            printf("collision\n");
            player.camera.velocityY = 0.0f;
        }

        glfwSetWindowShouldClose(window.window, windowShouldClose);
        window.keyInput();
        window.updateInputMode();
        window.update();
    }

    window.destroy();
    return 0;
}
 */
