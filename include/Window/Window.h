#pragma once

#include <GLFW/glfw3.h>

#include "Camera/Camera.h"

namespace EngineLN 
{

    void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    class Window {
    public: 

        bool debugVisible = false; 
        bool chatVisible = false;
        bool gameVisible = false;
        GLFWwindow* window = nullptr;

        bool init(int width, int height, const char* name);
        void update();
        void destroy();

        bool shouldClose();

        void keyInput();

        void updateInputMode();
        void setInputMode(GLenum inputMode, Camera& camera);

    private:

        bool inputMode;

    };

}

