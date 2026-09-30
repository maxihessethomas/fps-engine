#pragma once

#include <cmath>

struct Vec2 { float x, y; };
struct Vec4 { float x, y, z, w; };
struct Mat4 { float matrix[4][4]; };

class Vec3 {
public:
    float x, y, z;

    Mat4 mat4Translate(Vec3 translation) const {
        return {
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1
        };
    }
};

// MISC //
float aspect(int width, int height) {
    return (float)width/ (float)height;
}
inline float degreesToRadians(float angle) {
    return angle * (M_PI / 180);
}
inline float vec3Length(Vec3 vector) {
    return (float){
        sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z))
    };
}

// VEC3 //
inline Vec3 vec3Normalize(Vec3 vector) {
    float length = vec3Length(vector);
    if (length == 0) {
        return (Vec3){0};
    }
    return (Vec3){
        .x = vector.x / length,
        .y = vector.y / length,
        .z = vector.z / length
    };
}
inline Vec3 vec3Cross(Vec3 a, Vec3 b) {
    return (Vec3) {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}
inline Vec3 vec4ToNdc(Vec4 vector) {
    return (Vec3){
        .x = vector.x / vector.w,
        .y = vector.y / vector.w,
        .z = vector.z / vector.w
    };
}

// VEC4 //
inline Vec4 mat4MulVec4(Mat4 matrix, Vec4 vector) {
    return (Vec4){
        .x = 
            matrix.matrix[0][0] * vector.x +
            matrix.matrix[0][1] * vector.y +
            matrix.matrix[0][2] * vector.z +
            matrix.matrix[0][3] * vector.w,
        .y = 
            matrix.matrix[1][0] * vector.x +
            matrix.matrix[1][1] * vector.y +
            matrix.matrix[1][2] * vector.z +
            matrix.matrix[1][3] * vector.w,
        .z = 
            matrix.matrix[2][0] * vector.x +
            matrix.matrix[2][1] * vector.y +
            matrix.matrix[2][2] * vector.z +
            matrix.matrix[2][3] * vector.w,
        .w = 
        	matrix.matrix[3][0] * vector.x +
            matrix.matrix[3][1] * vector.y +
            matrix.matrix[3][2] * vector.z +
            matrix.matrix[3][3] * vector.w,
    };
}

// MAT4 //
inline Mat4 mat4Identity() {
    return (Mat4){
        .matrix[0][0] = 1.0f,
        .matrix[1][1] = 1.0f,
        .matrix[2][2] = 1.0f,
        .matrix[3][3] = 1.0f
    };
}
inline Mat4 mat4Mul(Mat4 a, Mat4 b) {
    Mat4 result = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.matrix[i][j] = 
                a.matrix[i][0] * b.matrix[0][j] +
                a.matrix[i][1] * b.matrix[1][j] +
                a.matrix[i][2] * b.matrix[2][j] +
                a.matrix[i][3] * b.matrix[3][j];
        }
    }
    return result;
}