#pragma once

#include <PrecompiledHeaders.h>

namespace EngineLN 
{

    struct Vec2 { float x, y; };
    struct intVec2 { int x, y; };
    struct Vec4 { float x, y, z, w; };
    struct Mat4 { float matrix[4][4]; };
    struct Vec3 { float x, y, z; };

    float aspect(int width, int height);
    float degreesToRadians(float angle);

    float vec3Length(Vec3 vector);
    float vec3Distance(Vec3 a, Vec3 b);

    Vec3 vec3Normalize(Vec3 vector);
    Vec3 vec3Cross(Vec3 a, Vec3 b);
    Vec3 vec4ToNdc(Vec4 vector);
    Vec3 vec4ToVec3(Vec4 vector);

    Vec4 vec3ToVec4(Vec3 vector);
    Vec4 mat4MulVec4(Mat4 matrix, Vec4 vector);

    Mat4 mat4Scale(float scalar);
    Mat4 mat4Identity();
    Mat4 mat4Translate(Vec3 vector);
    Mat4 mat4RotateZ(float degrees);
    Mat4 mat4RotateX(float degrees);
    Mat4 mat4RotateY(float degrees);
    Mat4 mat4Mul(Mat4 a, Mat4 b);

}

