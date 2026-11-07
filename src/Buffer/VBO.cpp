#include "Buffer.h"

EngineLN::VBO::VBO() { glGenBuffers(1, &id); };
EngineLN::VBO::~VBO() { glDeleteBuffers(1, &id); };

void EngineLN::VBO::bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }
void EngineLN::VBO::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void EngineLN::VBO::init(const std::vector<Vertex> data, size_t size) {       
    glBufferData(GL_ARRAY_BUFFER, size, data.data(), GL_STATIC_DRAW);  
}
void EngineLN::VBO::init(const std::vector<float> data, size_t size) {
    glBufferData(GL_ARRAY_BUFFER, size, data.data(), GL_STATIC_DRAW);
}