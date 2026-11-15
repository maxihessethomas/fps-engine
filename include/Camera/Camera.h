#pragma once

#include <GLFW/glfw3.h>

#include "Math/Math.h"

namespace EngineLN
{
    
    enum VIEW_TYPE {
        ROTATION,
        TRANSLATION,
        ROTATION_AND_TRANSLATION
    };

    enum VIEW_POSITION {
        CAMERA,
        SCENE
    };

    class Camera {
    public:

        Vec3 position = {0, 0, 0};
        float yaw = 0.0f, pitch = 0.0f, roll = 0.0f, fov = 0.0f, near = 0.0f, far = 0.0f;
        float velocityY;
        float speed = 1.0f;

        void keyInput(GLFWwindow* window, float deltaTime);

        static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        void mouseInput(double xpos, double ypos, float sensitivity);

        Vec3 getForward() const;
        Vec3 getRight() const;
        Vec3 getUp() const;

        Mat4 view(VIEW_TYPE viewtype, VIEW_POSITION viewposition) const;
        Mat4 perspective(float fov, float aspect, float near, float far) const;

        void gravity(const float gravity, const float deltaTime);

    };

}
