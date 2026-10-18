#ifndef WINDOW_H
#define WINDOW_H

#include "glad.h"
#include "GLFW/glfw3.h"

#include "camera.hpp"

class Window {
public: 
    GLFWwindow* window = nullptr;

    bool init(int width, int height, const char* name) {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        window = glfwCreateWindow(width, height, name, NULL, NULL);

        if (!window) {
            printf("DEBUG: GLFWwindow failed to create");
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);

        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        glEnable(GL_DEPTH_TEST);

        return true;
    }

    bool shouldClose() {
        return glfwWindowShouldClose(window);
    }

    void keyInput() {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && inputMode) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    void updateInputMode() {
        static bool prevTab = false;
        bool tab = glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS;
        if (tab && !prevTab) {
            uiMode = !uiMode;
            if (uiMode) {
                inputMode = false;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                glfwSetCursorPosCallback(window, 0);
            } else {
                inputMode = true;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                glfwSetCursorPosCallback(window, Camera::mouseCallback);
            }
        }
        prevTab = tab;
    }

    void setInputMode(GLenum inputMode, Camera& camera) {
        glfwSetWindowUserPointer(window, &camera);
        glfwSetInputMode(window, GLFW_CURSOR, inputMode);
/*         if (inputMode != GLFW_CURSOR_NORMAL) {
            glfwSetCursorPosCallback(window, Camera::mouseCallback);
        } */
    }

    void update() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void destroy() {
        glfwDestroyWindow(window);
        glfwTerminate();
        printf("terminated");
    }

private:
    bool uiMode, inputMode;
};

#endif

