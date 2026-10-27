#include "include/PrecompiledHeaders.h"

#include "GLFW/glfw3.h"
#include "glad.h"
#include "vertex.h"

class VBO {
public:
    VBO() {
        glGenBuffers(1, &id);
    }

    void bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }
    void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    void init(const std::vector<Vertex> data, size_t size) {       
        glBufferData(GL_ARRAY_BUFFER, size, data.data(), GL_STATIC_DRAW);  
    }
    void init(const std::vector<float> data, size_t size) {
        glBufferData(GL_ARRAY_BUFFER, size, data.data(), GL_STATIC_DRAW);
    }

    ~VBO() {
        glDeleteBuffers(1, &id);
    }
private:
    unsigned int id;
};

class VAO {
public:

    VAO() {
        glGenVertexArrays(1, &id);
    }

    void bind() const { glBindVertexArray(id); }
    void unbind() const { glBindVertexArray(0); }

    void setAttribute(unsigned int index, int quant, size_t size, size_t offset, GLenum type) {
        glVertexAttribPointer(index, quant, type, GL_FALSE, size, (void*)offset);
        glEnableVertexAttribArray(index);
    }

    ~VAO() {
        glDeleteVertexArrays(1, &id);
    }
private: 
    unsigned int id;
};

class EBO {
public:

    EBO() {
        glGenBuffers(1, &id);
    }

    void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }
    void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    void init(const unsigned int* data, size_t size) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
    void init(const std::vector<unsigned int> data, size_t size) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data.data(), GL_STATIC_DRAW);
    }

    ~EBO() {
        glDeleteBuffers(1, &id);
    }

private:
    unsigned int id;
};
