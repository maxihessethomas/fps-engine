#include "Math/Math.h"

EngineLN::Mat4 EngineLN::mat4Scale(float scalar) {
	return {
        scalar, 0, 0, 0,
        0, scalar, 0, 0,
        0, 0, scalar, 0,
        0, 0, 0, 1
    };
}

EngineLN::Mat4 EngineLN::mat4Identity() {
    return (Mat4){
        .matrix[0][0] = 1.0f,
        .matrix[1][1] = 1.0f,
        .matrix[2][2] = 1.0f,
        .matrix[3][3] = 1.0f
    };
}

EngineLN::Mat4 EngineLN::mat4Translate(Vec3 vector) {
    return {
        1, 0, 0, vector.x,
        0, 1, 0, vector.y,
        0, 0, 1, vector.z,
        0, 0, 0, 1
    };
}

EngineLN::Mat4 EngineLN::mat4RotateZ(float degrees) {
	Mat4 result = mat4Identity();
	float cos0 = cosf(degreesToRadians(degrees));
	float sin0 = sinf(degreesToRadians(degrees));
	result.matrix[0][0] = cos0;
	result.matrix[0][1] = -sin0;
	result.matrix[1][0] = sin0;
	result.matrix[1][1] = cos0;
	return result;
}

EngineLN::Mat4 EngineLN::mat4RotateX(float degrees) {
	Mat4 result = mat4Identity();
	float cos0 = cosf(degreesToRadians(degrees));
	float sin0 = sinf(degreesToRadians(degrees));
	result.matrix[1][1] = cos0;
	result.matrix[1][2] = -sin0;
	result.matrix[2][1] = sin0;
	result.matrix[2][2] = cos0;
	return result;
}

EngineLN::Mat4 EngineLN::mat4RotateY(float degrees) {
	Mat4 result = mat4Identity();
	float cos0 = cosf(degreesToRadians(degrees));
	float sin0 = sinf(degreesToRadians(degrees));
	result.matrix[0][0] = cos0;
	result.matrix[0][2] = sin0;
	result.matrix[2][0] = -sin0;
	result.matrix[2][2] = cos0;
	return result;
}

EngineLN::Mat4 EngineLN::mat4Mul(Mat4 a, Mat4 b) {
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
