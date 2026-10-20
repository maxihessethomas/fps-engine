#ifndef WINDOW_H
#define WINDOW_H

#include "glad.h"
#include "GLFW/glfw3.h"

#include "camera.hpp"

class Window {
public: 

    bool debugVisible = false; 
    bool chatVisible = false;
    bool gameVisible = false;
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
/*         if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            
        } */
    }

    void enableBackFaceCulling() {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CW);  
    }

    void updateInputMode() {
        static bool prevescape = false;
        bool escape = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (escape && !prevescape) {
            debugVisible = !debugVisible;
            if (debugVisible) {
                inputMode = false;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                glfwSetCursorPosCallback(window, 0);
            } else {
                inputMode = true;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                glfwSetCursorPosCallback(window, Camera::mouseCallback);
            }
        }
        prevescape = escape;
    }

/* bool updateInputMode(Camera camera) {
        static bool prevescape = false;
        static bool prevT = false;
        bool escape = glfwGetKey(window, GLFW_KEY_escape) == GLFW_PRESS;
        bool T = glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;

        if (escape && !prevescape || T && !prevT) {
            uiVisible = !uiVisible;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window, 0);
            if (escape && !prevescape) {
                debug = !debug;
            }
            if (T && !prevT) {
                chat = !chat;
            }
            prevescape = escape;
            prevT
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window, Camera::mouseCallback);
        }

        if (escape && !prevescape) {
            debugVisible = !debugVisible;
            chatVisible = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window, 0);
        } else if (T && !prevT) {
            chatVisible = !chatVisible;
            debugVisible = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window, 0); 
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window, Camera::mouseCallback);
        }

        prevescape = escape;
        prevT = T;

    } */

    void setInputMode(GLenum inputMode, Camera& camera) {
        glfwSetWindowUserPointer(window, &camera);
        glfwSetInputMode(window, GLFW_CURSOR, inputMode);
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
    bool inputMode;
};

#endif

