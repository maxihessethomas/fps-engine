#pragma once

#include "include/PrecompiledHeaders.h"

#include "buffer.hpp"
#include "math.hpp"
#include "import.hpp"
#include "shader.hpp"
#include "vertex.h"
#include "texture.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "collision.hpp"

class Mesh {
public:
    GLenum drawMode;

    Mesh(
        const char* vertexShaderPath, const char* fragmentShaderPath,
        const char* objFilePath, const char* textureFilePath,
        Light light, 
        Camera camera, 
        int SCREEN_W, 
        int SCREEN_H
    );

    void draw(Camera camera, 
        Light light, 
        int SCREEN_W, int SCREEN_H, 
        VIEW_POSITION viewposition, Mat4 rotation
    ) const;

    float getHeightFromVertexBelow(Camera camera);
    std::array<Vec3, 8> getHitBoxVertices();

private:
    VAO VAO;
    VBO VBO;
    EBO EBO;
    int indexCount;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::string vertexShaderSource, fragmentShaderSource;
    size_t vertex_count;
    Shader shader;
    Texture texture;
};

Mesh::Mesh(const char* vertexShaderPath,
        const char* fragmentShaderPath,
        const char* objFilePath,
        const char* textureFilePath,
        Light light,
        Camera camera,
        int SCREEN_W,
        int SCREEN_H)
{
    vertexShaderSource = readShader(vertexShaderPath);
    fragmentShaderSource = readShader(fragmentShaderPath);
    shader.init(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

    readOBJ(objFilePath, vertices, indices);
    texture.init(textureFilePath);

    indexCount = indices.size();

    VAO.bind();
    
    VBO.bind();
    VBO.init(vertices, vertices.size() * sizeof(Vertex));

    EBO.bind();
    EBO.init(indices, indices.size() * sizeof(unsigned int));

    VAO.setAttribute(0, 3, sizeof(Vertex), 0, GL_FLOAT);
    VAO.setAttribute(1, 2, sizeof(Vertex), sizeof(Vec3), GL_FLOAT);
    VAO.setAttribute(2, 3, sizeof(Vertex), sizeof(Vec3) + sizeof(Vec2), GL_FLOAT);

    VAO.unbind();
    VBO.unbind();
    EBO.unbind();
}

void Mesh::draw(Camera camera, 
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

float Mesh::getHeightFromVertexBelow(Camera camera) {

}

/* bool IsObjectInObject(std::vector<Vertex> a, std::vector<Vertex> b) {
    for (size_t i = 0; i < vertex)
} */

/* std::array<Vec3, 8> getHitBoxVertices(std::vector<Vertex> vertices) {
    return {
        std::max_element(vertices.begin(), vertices.end(), [](const Vertex& a, const Vertex& b) { return a.position.x < b.position.x; }),
        std::max
    }   
} */
