#pragma once

#include "GLFW/glfw3.h"
#include "glad.h"

#include <vector>

class VBO {
public:
    unsigned int id;

    VBO() {
        glGenBuffers(1, &id);
    }

    void init(const std::vector<float> data, size_t size) {        
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, data.data(), GL_STATIC_DRAW);  
    }
    void init(const float* data, size_t size) {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }
    void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    ~VBO() {
        glDeleteBuffers(1, &id);
    }
};

class VAO {
public:
    unsigned int id;

    VAO() {
        glGenVertexArrays(1, &id);
    }

    void bind() const { glBindVertexArray(id); }
    void unbind() const { glBindVertexArray(0); }

    void setAttribute(unsigned int index, int quant, size_t size, size_t offset, GLenum type) {
        bind();
        glVertexAttribPointer(index, quant, type, GL_FALSE, size, (void*)offset);
        glEnableVertexAttribArray(index);
    }

    ~VAO() {
        glDeleteVertexArrays(1, &id);
    }
};

class EBO {
public:
    unsigned int id;

    EBO() {
        glGenBuffers(1, &id);
    }

    void init(const unsigned int* data, size_t size) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
    void init(std::vector<unsigned int> data, size_t size) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data.data(), GL_STATIC_DRAW);
    }

    void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }
    void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    ~EBO() {
        glDeleteBuffers(1, &id);
    }
};
