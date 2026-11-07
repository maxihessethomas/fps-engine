#include "Camera/Camera.h"

void Camera::keyInput(GLFWwindow* window, float deltaTime) {
    Vec3 forward = getForward();
    Vec3 right = getRight();

    forward.y = 0.0f;
    forward = vec3Normalize(forward);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position.x += speed * forward.x * deltaTime;
        position.z += speed * forward.z * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position.x -= speed * forward.x * deltaTime;
        position.z -= speed * forward.z * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position.x += speed * right.x * deltaTime;
        position.z += speed * right.z * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position.x -= speed * right.x * deltaTime;
        position.z -= speed * right.z * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position.y += speed * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        position.y -= speed * deltaTime;
        
    }
}

void Camera::mouseInput(double xpos, double ypos, float sensitivity) {
    static bool firstMouse;
    static float lastX;
    static float lastY;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }

    float dx = xpos - lastX;
    float dy = ypos - lastY;

    lastX = xpos;
    lastY = ypos;

    yaw += dx * sensitivity;
    pitch -= dy * sensitivity;

    if (pitch > degreesToRadians(89)) pitch = degreesToRadians(89);
    if (pitch < degreesToRadians(-89)) pitch = degreesToRadians(-89);
}

void Camera::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera) {
        camera->mouseInput(xpos, ypos, 0.002);
    }
}