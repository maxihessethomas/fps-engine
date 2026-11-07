#include "Buffer.h"

EngineLN::EBO::EBO() { glGenBuffers(1, &id); }
EngineLN::EBO::~EBO() { glDeleteBuffers(1, &id); }

void EngineLN::EBO::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }
void EngineLN::EBO::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

void EngineLN::EBO::init(const unsigned int* data, size_t size) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
void EngineLN::EBO::init(const std::vector<unsigned int> data, size_t size) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data.data(), GL_STATIC_DRAW);
}
