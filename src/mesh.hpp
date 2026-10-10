#pragma once

#include "buffer.hpp"
#include "math.hpp"
#include "import.hpp"
#include "shader.hpp"
#include "vertex.h"

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

    Mesh(const char* vertexShaderPath, const char* fragmentShaderPath) {
        vertexShaderSource = readShader(vertexShaderPath);
        fragmentShaderSource = readShader(fragmentShaderPath);
        shader.init(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

        VAO.bind();
        VBO.bind();
/*         vbo.init();
        ebo.bind();
        ebo.init(); */


        // x, y, z
        VAO.setAttribute(0, 3, 8 * sizeof(float), 0, GL_FLOAT);
        // u, v
        VAO.setAttribute(1, 2, 8 * sizeof(float), 3 * sizeof(float), GL_FLOAT);
        // nx, ny, nz
        VAO.setAttribute(2, 3, 8 * sizeof(float), 5 * sizeof(float), GL_FLOAT);

        VAO.unbind();

    }

    void draw() const {
        VAO.bind();
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }
};