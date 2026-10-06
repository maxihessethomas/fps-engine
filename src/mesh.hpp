#pragma once

#include "buffer.hpp"

#include <iostream>
#include <vector>

/* struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec3 texcoords;
}; */

class Mesh {
public:
    VertexArray vao;
    VertexBuffer vbo;
    ElementBuffer ebo;
    int indexCount;

    Mesh(const float* vertexData, size_t vertexSize, const unsigned int* indexData, size_t indexSize, int indexCount)
        : vbo(vertexData, vertexSize), ebo(indexData, indexSize), indexCount(indexCount) {
        vao.bind();
        vbo.bind();
        ebo.bind();


        // x, y, z
        vao.setAttribute(0, 3, 8 * sizeof(float), 0);
        // u, v
        vao.setAttribute(1, 2, 8 * sizeof(float), 3 * sizeof(float));
        // nx, ny, nz
        vao.setAttribute(2, 3, 8 * sizeof(float), 5 * sizeof(float));

        vao.unbind();
    }

    void draw() const {
        vao.bind();
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }
};

/* class Mesh {
public: 
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
    }

private:
    VertexArray VAO;
    VertexBuffer VBO;
    ElementBuffer EBO;

    VAO.bind();
    VBO.bind();
    
}; */