#pragma once

#include <PrecompiledHeaders.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

namespace EngineLN 
{

    class Texture {
    public: 

        unsigned int id;
        int width, height, nrChannels;

        Texture();
        
        void bind() const;
        void unbind() const;

        void init(const char* path);

    };

    class CubeMap {
    public: 

        unsigned int id;
        int width, height, nrChannels;

        CubeMap();

        void bind();
        void unbind();

        void init(const char* faces[6]);

    };

}
