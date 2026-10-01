#pragma once

#include "math.hpp"

struct Rectangle {
    Vec3 position;
    float width;
    float height;
};

int AABB(Rectangle a, Rectangle b) {
    
    if (a.position.x - (a.width / 2) >= b.position.x - (b.width / 2) &&
        a.position.x + (a.width / 2) <= b.position.x + (b.width / 2) &&
        a.position.y - (a.height / 2) >= b.position.y - (b.height) &&
        a.position.y <= b.position.y &&
        a.position.z - (a.width / 2) >= b.position.z - (b.width / 2) &&
        a.position.z + (a.width / 2) <= b.position.z + (b.width / 2)
    ) {
        return 1;
    }
    return 0;
}