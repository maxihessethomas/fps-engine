#pragma once

#include "buffer.hpp"
#include "math.hpp"
#include "import.hpp"
#include "shader.hpp"
#include "vertex.h"
#include "texture.hpp"
#include "camera.hpp"
#include "light.hpp"

#include <optional>
#include <vector>

class Mesh {
public:
    VAO VAO;
    VBO VBO;
    EBO EBO;
    int indexCount;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::string vertexShaderSource, fragmentShaderSource;
    Shader shader;
    Texture texture;

    Mesh(const char* vertexShaderPath, 
        const char* fragmentShaderPath, 
        const char* objFilePath,
        const char* textureFilePath
    ) {
        vertexShaderSource = readShader(vertexShaderPath);
        fragmentShaderSource = readShader(fragmentShaderPath);
        shader.init(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

        readOBJ(objFilePath, vertices, indices);
        texture.init(textureFilePath);

        indexCount = indices.size();

        VAO.bind();
        
        VBO.bind();
        VBO.init(vertices, vertices.size());

        EBO.bind();
        EBO.init(indices, indices.size());

        VAO.setAttribute(0, 3, sizeof(Vertex), 0, GL_FLOAT);
        VAO.setAttribute(1, 2, sizeof(Vertex), sizeof(Vec3), GL_FLOAT);
        VAO.setAttribute(2, 3, sizeof(Vertex), sizeof(Vec3) + sizeof(Vec2), GL_FLOAT);

        VAO.unbind();
        VBO.unbind();
        EBO.unbind();
    }

    void draw(Camera camera, Light light, int SCREEN_W, int SCREEN_H) const {
        texture.bind();
        VAO.bind();

        shader.use();
        shader.mat4Set("view", camera.view());
        shader.mat4Set("projection", camera.perspective(camera.fov, aspect(SCREEN_W, SCREEN_H), camera.near, camera.far));
        shader.mat4Set("model", mat4Identity());
        shader.vec3Set("viewPos", camera.position);
        shader.vec3Set("material.ambient", {0.0f, 0.0f, 0.0f});
        shader.vec3Set("material.diffuse", {1.0f, 1.0f, 1.0f});
        shader.vec3Set("material.specular", {0.5f, 0.5f, 0.5f});
        shader.floatSet("material.shininess", 32.0f);
        shader.vec3Set("light.position", light.position);
        shader.vec3Set("light.ambient",  light.ambient);
        shader.vec3Set("light.diffuse",  light.diffuse);
        shader.vec3Set("light.specular", light.specular);   

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

        VAO.unbind();
        texture.unbind();
    }
};