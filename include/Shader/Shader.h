#pragma once

#include <PrecompiledHeaders.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Math/Math.h"

namespace EngineLN 
{

    class Shader {
    public: 

        unsigned int vertexShader, fragmentShader, computeShader, shaderProgram;

        void createShader(unsigned int& shader, const char* shaderSource, GLenum type);
        void createProgram(unsigned int& program, std::vector<unsigned int> shader);

        Shader() {}
        ~Shader();

        void init(const char* vertexShaderSource, const char* fragmentShaderSource, const char* computeShaderSource);
        void init(const char* vertexShaderSource, const char* fragmentShaderSource);

        void use() const { glUseProgram(shaderProgram); }

        void mat4Set(const char* name, const Mat4 matrix) const;
        void vec3Set(const char* name, const Vec3 vector) const;
        void floatSet(const char* name, const float x) const;
        void intSet(const char* name, const int x) const;

    };
    
}

