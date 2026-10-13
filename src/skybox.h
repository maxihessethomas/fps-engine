#pragma once

#include "buffer.hpp"
#include "math.hpp"
#include "import.hpp"
#include "shader.hpp"
#include "vertex.h"
#include "texture.hpp"
#include "camera.hpp"
#include "light.hpp"

#include <vector>

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
    ) {
        vertexShaderSource = readShader(vertexShaderPath);
        fragmentShaderSource = readShader(fragmentShaderPath);
        shader.init(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
        
        skybox.bind();
        skybox.init(textureFilePaths);

        vertices = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
        };

        VAO.bind();

        VBO.bind();
        VBO.init(vertices, vertices.size() * sizeof(float));

        VAO.setAttribute(0, 3, sizeof(Vec3), 0, GL_FLOAT);

        skybox.unbind();
        VAO.unbind();
        VBO.unbind();
    }

    void draw(Camera camera,
            int SCREEN_W,
            int SCREEN_H
    ) {
        glDepthFunc(GL_LEQUAL);

        skybox.bind();
        VAO.bind();

        shader.use();
        shader.mat4Set("projection", camera.perspective(camera.fov, aspect(SCREEN_W, SCREEN_H), camera.near, camera.far));
        shader.mat4Set("view", camera.view(ROTATION));

        glActiveTexture(GL_TEXTURE0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        VAO.unbind();
        skybox.unbind();

        glDepthFunc(GL_LESS);
    }

};