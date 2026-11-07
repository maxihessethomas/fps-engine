/* #pragma once

class Player {
public:
    Vec3 position;
    Camera camera;
    Mesh hitbox{"assets/cube.obj"};

    Player(Vec3 position);
    
    bool AreVerticesColliding(const Mesh& mesh);

};

Player::Player(Vec3 position) {
    position = position;
    camera.position = position;
    camera.fov = degreesToRadians(60);
    camera.near = 0.1f;
    camera.far = 100.0f;
}

bool Player::AreVerticesColliding(const Mesh& mesh) 
{
    float distance;
    for (size_t indexA = 0; indexA < hitbox.vertices.size(); indexA++) {
        for (size_t indexB = 0; indexB < mesh.vertices.size(); indexB++) {
            distance = vec3Distance(hitbox.vertices[indexA].position, mesh.vertices[indexB].position);
            if (distance <= 0.1f) {
                return true;
            }
        }
    }
    return false;
} */