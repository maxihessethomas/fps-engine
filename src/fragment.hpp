#pragma once

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