
#include <iostream>

struct Light {
    Vec3 position;

    Vec3 ambient;
    Vec3 diffuse;
    Vec3 specular;
};

struct Rectangle {
    Vec3 position;
    int width;
    int height;
};

int AABB(Rectangle a, Rectangle b) {
    if (a.position.x - (a.width / 2) >= b.position.x - (b.width / 2) &&
        a.position.x + (a.width / 2) <= b.position.x + (b.width / 2) &&
        a.position.y - (a.width / 2) >= b.position.y - (b.height) &&
        a.position.y <= b.position.y &&
        a.position.z - (a.width / 2) >= b.position.z - (b.width / 2) &&
        a.position.z + (a.width / 2) <= b.position.z + (b.width / 2)
    ) {
        return 1;
    } else {
        return 0;
    }
}

