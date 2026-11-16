#pragma once

#include <glad/glad.h>

#include <PrecompiledHeaders.h>

#include "Vertex/Vertex.h"

namespace EngineLN
{

    class VBO {
        public:

        VBO();

        void bind() const;
        void unbind() const;

        void init(const std::vector<Vertex> data, size_t size);
        void init(const std::vector<float> data, size_t size);

        ~VBO();

        private:

        unsigned int id;

    };

    class VAO {
        public:

        VAO();

        void bind() const;
        void unbind() const;

        void setAttribute(unsigned int index, int quant, size_t size, size_t offset, GLenum type);

        ~VAO();

        public:

        unsigned int id;

    };

    class EBO {
    public:

    EBO();

    void bind() const;
    void unbind() const;

    void init(const unsigned int* data, size_t size);
    void init(const std::vector<unsigned int> data, size_t size);

    ~EBO();

    public:

    unsigned int id;

    };

}
