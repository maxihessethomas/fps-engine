#include "Window/Window.h"

void EngineLN::Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool EngineLN::Window::init(int width, int height, const char* name) {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glEnable(GL_DEPTH_TEST);

    return true;
}

bool EngineLN::Window::shouldClose() {
    return glfwWindowShouldClose(window);
}

void EngineLN::Window::keyInput() {
}

void EngineLN::Window::enableBackFaceCulling() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);  
}

void EngineLN::Window::updateInputMode() {
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

void EngineLN::Window::setInputMode(GLenum inputMode, Camera& camera) {
    glfwSetWindowUserPointer(window, &camera);
    glfwSetInputMode(window, GLFW_CURSOR, inputMode);
}

void EngineLN::Window::update() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void EngineLN::Window::destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
    printf("terminated");
}
