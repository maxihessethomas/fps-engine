#pragma once

#include <PrecompiledHeaders.h>

#include "Import/Import.h"

namespace EngineLN 
{

    class Skybox {
    public: 
        VAO VAO;
        VBO VBO;
        EBO EBO;
        std::vector<float> vertices;
        std::string vertexShaderSource, fragmentShaderSource;
        Shader shader;
        CubeMap skybox;

        Skybox(const char* vertexShaderPath,
            const char* fragmentShaderPath,
            const char* textureFilePaths[6]
        );

        void draw(Camera camera,
                int SCREEN_W,
                int SCREEN_H
        );

    };
    
}

