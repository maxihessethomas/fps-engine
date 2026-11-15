#include "Camera/Camera.h"

EngineLN::Vec3 EngineLN::Camera::getForward() const {
    return {
        cosf(pitch) * sinf(yaw),
        sinf(pitch),
        -cosf(pitch) * cosf(yaw)
    };
}

EngineLN::Vec3 EngineLN::Camera::getRight() const {
    return {
        cosf(yaw),
        0.0f,
        sinf(yaw)
    };
}

EngineLN::Vec3 EngineLN::Camera::getUp() const {
    return vec3Normalize(vec3Cross(getRight(), getForward()));
}

