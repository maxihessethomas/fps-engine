#include "Math/Math.h"

EngineLN::Vec4 EngineLN::vec3ToVec4(Vec3 vector) {
    return {
        vector.x,
        vector.y, 
        vector.z,
        1.0f
    };
}

EngineLN::Vec4 EngineLN::mat4MulVec4(Mat4 matrix, Vec4 vector) {
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