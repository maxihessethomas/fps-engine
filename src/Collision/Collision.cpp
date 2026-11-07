#include "Collision/Collision.h"

bool EngineLN::AreVerticesCollidiing(const Mesh& a, const Mesh& b) {
    float distance;
    for (size_t indexA = 0; indexA < a.vertices.size(); indexA++) {
        for (size_t indexB = 0; indexB < b.vertices.size(); indexB++) {
            distance = vec3Distance(a.vertices[indexA].position, b.vertices[indexB].position);
            if (distance <= 0.1f) {
                return true;
            }
        }
    }
    return false;
}


