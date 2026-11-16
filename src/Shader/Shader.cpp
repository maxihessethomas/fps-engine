#include "Shader/Shader.h"

class Shader {
public:
    unsigned int vertexShader, fragmentShader, computeShader, shaderProgram;

    void createShader(unsigned int& shader, const char* shaderSource, GLenum type) {
        shader = glCreateShader(type);
        glShaderSource(shader, 1, &shaderSource, NULL);
        glCompileShader(shader);
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cout << "SHADER COMPILE ERROR:\n" << infoLog << '\n';
        }
    }

    void createProgram(unsigned int& program, std::vector<unsigned int> shader) {
        program = glCreateProgram();
        for (int i = 0; i < shader.size(); i++) {
            glAttachShader(program, shader[i]);
        }
        glLinkProgram(program);
        int success;
        char infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    }

    Shader() {}

    void init(const char* vertexShaderSource, const char* fragmentShaderSource, const char* computeShaderSource) {
        createShader(vertexShader, vertexShaderSource, GL_VERTEX_SHADER);
        createShader(fragmentShader, fragmentShaderSource, GL_FRAGMENT_SHADER);
        createShader(computeShader, computeShaderSource, GL_COMPUTE_SHADER);
        std::vector<unsigned int> shaders{vertexShader, fragmentShader, computeShader};
        createProgram(shaderProgram, shaders);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteShader(computeShader);
    }

    void init(const char* vertexShaderSource, const char* fragmentShaderSource) {
        createShader(vertexShader, vertexShaderSource, GL_VERTEX_SHADER);
        createShader(fragmentShader, fragmentShaderSource, GL_FRAGMENT_SHADER);
        std::vector<unsigned int> shaders{vertexShader, fragmentShader};
        createProgram(shaderProgram, shaders);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteShader(computeShader);
    }

    void use() const { glUseProgram(shaderProgram); }

    void mat4Set(const char* name, const Mat4 matrix) const {
        unsigned int location = glGetUniformLocation(shaderProgram, name);
        glUniformMatrix4fv(location, 1, GL_TRUE, &matrix.matrix[0][0]);
    }

    void vec3Set(const char* name, const Vec3 vector) const {
        unsigned int location = glGetUniformLocation(shaderProgram, name);
        glUniform3f(location, vector.x, vector.y, vector.z);
    }

    void floatSet(const char* name, const float x) const {
        unsigned int location = glGetUniformLocation(shaderProgram, name);
        glUniform1f(location, x);
    }

    void intSet(const char* name, const int x) const {
        unsigned int location = glGetUniformLocation(shaderProgram, name);
        glUniform1i(location, x);
    }

    ~Shader() {
        glDeleteProgram(shaderProgram);
    }
};
