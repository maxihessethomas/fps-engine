#pragma once

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