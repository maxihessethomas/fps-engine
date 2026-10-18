#include <stdio.h>

#include "glad.h"
#include "GLFW/glfw3.h"

#include "window.h"

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
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    void update() {
        keyInput();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ~Window() {
        if (window) {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
    }
};