#include "Mesh/Mesh.h"

EngineLN::Mesh::Mesh(const char* vertexShaderPath,
        const char* fragmentShaderPath,
        const char* objFilePath,
        const char* textureFilePath)
{
    vertexShaderSource = readShader(vertexShaderPath);
    fragmentShaderSource = readShader(fragmentShaderPath);
    shader.init(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

    readOBJ(objFilePath, vertices, indices);
    texture.init(textureFilePath);

    indexCount = indices.size();

    VAO.bind();
    
    VBO.bind();
    VBO.init(vertices, vertices.size() * sizeof(Vertex));

    EBO.bind();
    EBO.init(indices, indices.size() * sizeof(unsigned int));

    VAO.setAttribute(0, 3, sizeof(Vertex), 0, GL_FLOAT);
    VAO.setAttribute(1, 2, sizeof(Vertex), sizeof(Vec3), GL_FLOAT);
    VAO.setAttribute(2, 3, sizeof(Vertex), sizeof(Vec3) + sizeof(Vec2), GL_FLOAT);

    VAO.unbind();
    VBO.unbind();
    EBO.unbind();
}

EngineLN::Mesh::Mesh(const char* objFilePath) {
    readOBJ(objFilePath, vertices, indices);

    indexCount = indices.size();

    VAO.bind();
    
    VBO.bind();
    VBO.init(vertices, vertices.size() * sizeof(Vertex));

    EBO.bind();
    EBO.init(indices, indices.size() * sizeof(unsigned int));

    VAO.setAttribute(0, 3, sizeof(Vertex), 0, GL_FLOAT);
    VAO.setAttribute(1, 2, sizeof(Vertex), sizeof(Vec3), GL_FLOAT);
    VAO.setAttribute(2, 3, sizeof(Vertex), sizeof(Vec3) + sizeof(Vec2), GL_FLOAT);

    VAO.unbind();
    VBO.unbind();
    EBO.unbind();
}
