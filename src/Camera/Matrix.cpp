#include "Camera/Camera.h"

EngineLN::Mat4 EngineLN::Camera::view(VIEW_TYPE viewtype, VIEW_POSITION viewposition) const {
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

EngineLN::Mat4 EngineLN::Camera::perspective(float fov, float aspect, float near, float far) const {
    Mat4 result = {0};

    float f = 1.0f / tanf(fov / 2.0f);

    result.matrix[0][0] = f / aspect;
    result.matrix[1][1] = f;

    result.matrix[2][2] = (far + near) / (near - far);

    result.matrix[3][2] = -1.0f;
    result.matrix[2][3] = (2.0f * far * near) / (near - far);

    return result;
}