#pragma once

#include "GLFW/glfw3.h"
#include "glad.h"

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
