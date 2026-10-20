#pragma once

enum VIEW_TYPE {
    ROTATION,
    TRANSLATION,
    ROTATION_AND_TRANSLATION
};

enum VIEW_POSITION {
    CAMERA,
    SCENE
};

#include "mesh.hpp"

class Camera {
public:
    Vec3 position = {0, 0, 0};
    float yaw = 0.0f, pitch = 0.0f, roll = 0.0f, fov = 0.0f, near = 0.0f, far = 0.0f;

    float velocityY;
    float speed = 1.0f;

    void keyInput(GLFWwindow* window, float deltaTime);

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
        if (camera) {
            camera->mouseInput(xpos, ypos, 0.002);
        }
    }

    void mouseInput(double xpos, double ypos, float sensitivity) {
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

    void print() const {
        std::cout << "Position: (" << position.x << " " << position.y << " " << position.z << ")"
        << " Yaw: " << yaw << " Pitch: " << pitch << " Roll: " << roll << " VelocityY: " << "\n";
    }

    Vec3 getForward() const {
        return {
            cosf(pitch) * sinf(yaw),
            sinf(pitch),
            -cosf(pitch) * cosf(yaw)
        };
    }

    Vec3 getRight() const {
        return {
            cosf(yaw),
            0.0f,
            sinf(yaw)
        };
    }

    Vec3 getUp() const {
        return vec3Normalize(vec3Cross(getRight(), getForward()));
    }

    Mat4 view(VIEW_TYPE viewtype, VIEW_POSITION viewposition) const;

    Mat4 perspective(float fov, float aspect, float near, float far) const {
        Mat4 result = {0};

        float f = 1.0f / tanf(fov / 2.0f);

        result.matrix[0][0] = f / aspect;
        result.matrix[1][1] = f;

        result.matrix[2][2] = (far + near) / (near - far);

        result.matrix[3][2] = -1.0f;
        result.matrix[2][3] = (2.0f * far * near) / (near - far);

        return result;
    }

    void gravity(const float gravity, const float deltaTime) {
        if (deltaTime > 0.1) {
            velocityY += gravity * 0.1;
            position.y -= velocityY * 0.1;
        } else {
            velocityY += gravity * deltaTime;
            position.y -= velocityY * deltaTime;
        };
    }
};

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

Mat4 Camera::view(VIEW_TYPE viewtype, VIEW_POSITION viewposition) const {
    Vec3 forward = getForward();
    Vec3 right = getRight();
    Vec3 up = getUp();

    if (viewtype == ROTATION) {
        if (viewposition == CAMERA) {
            return {
                right.x, right.y, right.z, 0,
                up.x, up.y, up.z, 0,
                forward.x, forward.y, forward.z, 0,
                0, 0, 0, 1
            };
        } else {
            return {
                right.x, right.y, right.z, 0,
                up.x, up.y, up.z, 0,
                -forward.x, -forward.y, -forward.z, 0,
                0, 0, 0, 1
            }; 
        }
    }

    if (viewtype == TRANSLATION) {
        return {
            1, 0, 0, -position.x,
            0, 1, 0, -position.y,
            0, 0, 1, -position.z,
            0, 0, 0, 1
        };
    }

    if (viewtype == ROTATION_AND_TRANSLATION) {
        Mat4 rotation = {
            right.x, right.y, right.z, 0,
            up.x, up.y, up.z, 0,
            -forward.x, -forward.y, -forward.z, 0,
            0, 0, 0, 1
        };
        
        Mat4 translation = {
            1, 0, 0, -position.x,
            0, 1, 0, -position.y,
            0, 0, 1, -position.z,
            0, 0, 0, 1
        };

        return mat4Mul(rotation, translation);
    }
}

/* void gravity(Camera camera, float gravity, float deltaTime) {
    if (deltaTime > 0.1) {
        camera.velocityY += speed * gravity * 0.1;
        camera.position.y -= speed * camera.velocityY * 0.1;
    } else {
        camera.velocityY += speed * gravity * deltaTime;
        camera.position.y -= speed * camera.velocityY * deltaTime;
    };
} */