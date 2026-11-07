#include "Buffer.h"

EngineLN::VAO::VAO() { glGenVertexArrays(1, &id); }
EngineLN::VAO::~VAO() { glDeleteVertexArrays(1, &id); }

void EngineLN::VAO::bind() const { glBindVertexArray(id); }
void EngineLN::VAO::unbind() const { glBindVertexArray(0); }

void EngineLN::VAO::setAttribute(unsigned int index, int quant, size_t size, size_t offset, GLenum type) {
    glVertexAttribPointer(index, quant, type, GL_FALSE, size, (void*)offset);
    glEnableVertexAttribArray(index);
}

