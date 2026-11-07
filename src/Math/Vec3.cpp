#include "Math/Math.h"

EngineLN::Vec3 EngineLN::vec3Normalize(Vec3 vector) {
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

EngineLN::Vec3 EngineLN::vec3Cross(Vec3 a, Vec3 b) {
    return (Vec3) {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

EngineLN::Vec3 EngineLN::vec4ToNdc(Vec4 vector) {
    return (Vec3){
        .x = vector.x / vector.w,
        .y = vector.y / vector.w,
        .z = vector.z / vector.w
    };
}

EngineLN::Vec3 EngineLN::vec4ToVec3(Vec4 vector) {
    return {
        vector.x,
        vector.y,
        vector.z
    };
}
