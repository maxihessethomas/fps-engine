#include "Mesh/Mesh.h"

void EngineLN::Mesh::draw(Camera camera, 
    Light light, 
    int SCREEN_W, 
    int SCREEN_H, 
    VIEW_POSITION viewposition,
    Mat4 model
)
    const 
{
    texture.bind();
    VAO.bind();

    if (viewposition == SCENE) {
        shader.use();
        shader.mat4Set("projection", camera.perspective(camera.fov, aspect(SCREEN_W, SCREEN_H), camera.near, camera.far));
        shader.mat4Set("model", model); 
        shader.mat4Set("view", camera.view(ROTATION_AND_TRANSLATION, SCENE));
        shader.vec3Set("viewPos", camera.position); 
        shader.vec3Set("material.ambient", {1.0f, 1.0f, 1.0f});
        shader.vec3Set("material.diffuse", {1.0f, 1.0f, 1.0f});
        shader.vec3Set("material.specular", {1.0f, 1.0f, 1.0f});
        shader.floatSet("material.shininess", 32.0f);
        shader.vec3Set("light.position", light.position);
        shader.vec3Set("light.ambient",  light.ambient);
        shader.vec3Set("light.diffuse",  light.diffuse);
        shader.vec3Set("light.specular", light.specular);
    }

    if (viewposition == CAMERA) {
        shader.use();
        shader.mat4Set("projection", camera.perspective(camera.fov, aspect(SCREEN_W, SCREEN_H), camera.near, camera.far));
        shader.mat4Set("model", model); 
        shader.mat4Set("view", mat4Identity());
        shader.vec3Set("viewPos", {0.0f, 0.0f, 0.0f});
        shader.vec3Set("material.ambient", {1.0f, 1.0f, 1.0f});
        shader.vec3Set("material.diffuse", {1.0f, 1.0f, 1.0f});
        shader.vec3Set("material.specular", {1.0f, 1.0f, 1.0f});
        shader.floatSet("material.shininess", 100.0f);
        shader.vec3Set("light.position", vec4ToVec3(
            mat4MulVec4(camera.view(ROTATION_AND_TRANSLATION, CAMERA), 
            {light.position.x, light.position.y, light.position.z, 1.0f})));
        shader.vec3Set("light.ambient",  light.ambient);
        shader.vec3Set("light.diffuse",  light.diffuse);
        shader.vec3Set("light.specular", light.specular);
    }

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    VAO.unbind();
    texture.unbind();
}
