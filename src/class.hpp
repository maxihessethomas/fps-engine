#pragma once

#include "math.hpp"
#include "lib.h"

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

/* struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 texcoords;
}; */

class VertexBuffer {
public: 
    unsigned int id;

    VertexBuffer(const float* data, size_t size) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }
    void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    ~VertexBuffer() {
        glDeleteBuffers(1, &id);
    }
};

class VertexArray {
public:
    unsigned int id;

    VertexArray() {
        glGenVertexArrays(1, &id);
    }

    void bind() const { glBindVertexArray(id); }
    void unbind() const { glBindVertexArray(0); }

    void setAttribute(unsigned int index, int quant, size_t size, size_t offset) {
        bind();
        glVertexAttribPointer(index, quant, GL_FLOAT, GL_FALSE, size, (void*)offset);
        glEnableVertexAttribArray(index);
    }

    ~VertexArray() {
        glDeleteVertexArrays(1, &id);
    }
};

class ElementBuffer {
public:
    unsigned int id;

    ElementBuffer(const unsigned int* data, size_t size) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }
    void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    ~ElementBuffer() {
        glDeleteBuffers(1, &id);
    }
};

class Shader {
public: 
    unsigned int vertexShader, fragmentShader, shaderProgram;

    void createShader(unsigned int& shader, const char* shaderSource, GLenum type) {
        shader = glCreateShader(type);
        glShaderSource(shader, 1, &shaderSource, NULL);
        glCompileShader(shader);
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cout << "SHADER COMPILE ERROR:\n" << infoLog << '\n';
        }
    }

    void createProgram(unsigned int& program, unsigned int shader[2]) {
        program = glCreateProgram();
        glAttachShader(program, shader[0]);
        glAttachShader(program, shader[1]);
        glLinkProgram(program);
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        } 
    }
    
    Shader(const char* vertexShaderSource, const char* fragmentShaderSource) {
        createShader(vertexShader, vertexShaderSource, GL_VERTEX_SHADER);
        createShader(fragmentShader, fragmentShaderSource, GL_FRAGMENT_SHADER);
        unsigned int shaders[2] = {vertexShader, fragmentShader};
        createProgram(shaderProgram, shaders);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void use() const { glUseProgram(shaderProgram); }

    void mat4Set(const char* name, const Mat4 matrix) const {
        unsigned int location = glGetUniformLocation(shaderProgram, name);
        glUniformMatrix4fv(location, 1, GL_TRUE, &matrix.matrix[0][0]);
    }

    void vec3Set(const char* name, const Vec3 vector) const {
        unsigned int location = glGetUniformLocation(shaderProgram, name);
        glUniform3f(location, vector.x, vector.y, vector.z);
    }

    void floatSet(const char* name, const float x) {
        unsigned int location = glGetUniformLocation(shaderProgram, name);
        glUniform1f(location, x);
    }

    void intSet(const char* name, const int x) {
        unsigned int location = glGetUniformLocation(shaderProgram, name);
        glUniform1i(location, x);
    }

    ~Shader() {
        glDeleteProgram(shaderProgram);
    }
};

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
