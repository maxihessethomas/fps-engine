#include "Camera/Camera.h"

Vec3 Camera::getForward() const {
    return {
        cosf(pitch) * sinf(yaw),
        sinf(pitch),
        -cosf(pitch) * cosf(yaw)
    };
}

Vec3 Camera::getRight() const {
    return {
        cosf(yaw),
        0.0f,
        sinf(yaw)
    };
}

Vec3 Camera::getUp() const {
    return vec3Normalize(vec3Cross(getRight(), getForward()));
}

