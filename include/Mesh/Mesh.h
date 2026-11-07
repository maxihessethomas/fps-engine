#pragma once

#include <PrecompiledHeaders.h>
#include <GLFW/glfw3.h>

#include "Vertex/Vertex.h"
#include "Camera/Camera.h"
#include "Light/Light.h"
#include "Buffer/Buffer.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"

namespace EngineLN 
{

    class Mesh {
    public:
        GLenum drawMode;
        std::vector<Vertex> vertices;

        Mesh(
            const char* vertexShaderPath, const char* fragmentShaderPath,
            const char* objFilePath, const char* textureFilePath
        );
        Mesh(const char* objFilePath);

        void draw(Camera camera, 
            Light light, 
            int SCREEN_W, int SCREEN_H, 
            VIEW_POSITION viewposition, Mat4 rotation
        ) const;

        float getHeightFromVertexBelow(Camera camera);
        std::array<Vec3, 8> getHitBoxVertices();

    private:
        VAO VAO;
        VBO VBO;
        EBO EBO;
        int indexCount;
        std::vector<unsigned int> indices;
        std::string vertexShaderSource, fragmentShaderSource;
        size_t vertex_count;
        Shader shader;
        Texture texture;
    };

}
