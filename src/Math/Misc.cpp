#include "Math/Math.h"

float EngineLN::aspect(int width, int height) { return width / height; }
float EngineLN::degreesToRadians(float angle) { return angle * (M_PI / 180); }

float EngineLN::vec3Length(Vec3 vector) 
{
    return {
        sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z))
    };
}

float EngineLN::vec3Distance(Vec3 a, Vec3 b) {
    return {
        sqrt(
            (a.x - b.x) * (a.x - b.x) +
            (a.y - b.y) * (a.y - b.y) +
            (a.z - b.z) * (a.z - b.z)
        )
    };
}


