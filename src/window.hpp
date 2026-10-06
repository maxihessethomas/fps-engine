#pragma once

#include "GLFW/glfw3.h"
#include "glad.h"

class Window {
    Window(int width, int height, const char* name) {
        glfwCreateWindow(width, height, name, NULL, NULL);
    }
};