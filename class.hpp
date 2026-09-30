#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include "glad.h"
#include "lib/glfw/include/GLFW/glfw3.h"
#include "lib/stb/stb_image.h"

#include "math.hpp"

#include <iostream>

struct Light {
    Vec3 position;

    Vec3 ambient;
    Vec3 diffuse;
    Vec3 specular;
};

struct Rectangle {
    Vec3 position;
    int width;
    int height;
};

int AABB(Rectangle a, Rectangle b) {
    if (a.position.x - (a.width / 2) >= b.position.x - (b.width / 2) &&
        a.position.x + (a.width / 2) <= b.position.x + (b.width / 2) &&
        a.position.y - (a.width / 2) >= b.position.y - (b.height) &&
        a.position.y <= b.position.y &&
        a.position.z - (a.width / 2) >= b.position.z - (b.width / 2) &&
        a.position.z + (a.width / 2) <= b.position.z + (b.width / 2)
    ) {
        return 1;
    } else {
        return 0;
    }
}

class Camera {
public:
    Vec3 position = {0, 0, 0};
    float yaw = 0.0f, pitch = 0.0f, roll = 0.0f, fov = 0.0f, near = 0.0f, far = 0.0f;

    float velocityY;

    void keyInput(GLFWwindow* window, float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        Vec3 forward = getForward();
        Vec3 right = getRight();

        // Don't let looking up/down affect horizontal movement
        forward.y = 0.0f;
        forward = vec3Normalize(forward);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            position.x += forward.x * deltaTime;
            position.z += forward.z * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            position.x -= forward.x * deltaTime;
            position.z -= forward.z * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            position.x += right.x * deltaTime;
            position.z += right.z * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            position.x -= right.x * deltaTime;
            position.z -= right.z * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            position.y += 0.5f * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            position.y -= 0.5f * deltaTime;
        }
    }

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
        if (camera) {
            camera->mouseInput(xpos, ypos, 0.002);
        }
    }

    void mouseInput(double xpos, double ypos, float sensitivity) {
        static float lastX;
        static float lastY;
        static bool firstMouse = true;

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
            return;
        }

        float dx = xpos - lastX;
        float dy = ypos - lastY;

        lastX = xpos;
        lastY = ypos;

        yaw += dx * sensitivity;
        pitch -= dy * sensitivity;

        if (pitch > degreesToRadians(89)) pitch = degreesToRadians(89);
        if (pitch < degreesToRadians(-89)) pitch = degreesToRadians(-89);
    }

    void print() const {
        std::cout << "Position: (" << position.x << " " << position.y << " " << position.z << ")"
        << " Yaw: " << yaw << " Pitch: " << pitch << " Roll: " << roll << " VelocityY: " << "\n";
    }

    Vec3 getForward() const {
        return {
            cosf(pitch) * sinf(yaw),
            sinf(pitch),
            -cosf(pitch) * cosf(yaw)
        };
    }

    Vec3 getRight() const {
        return {
            cosf(yaw),
            0.0f,
            sinf(yaw)
        };
    }

    Vec3 getUp() const {
        return vec3Normalize(vec3Cross(getRight(), getForward()));
    }

    Mat4 view() const {
        Vec3 forward = getForward();
        Vec3 right = getRight();
        Vec3 up = getUp();

        Mat4 rotation = {
            right.x, right.y, right.z, 0,
            up.x, up.y, up.z, 0,
            -forward.x, -forward.y, -forward.z, 0,
            0, 0, 0, 1
        };

        Mat4 translation = {
            1, 0, 0, -position.x,
            0, 1, 0, -position.y,
            0, 0, 1, -position.z,
            0, 0, 0, 1
        };

        return mat4Mul(rotation, translation);
    }

    Mat4 perspective(float fov, float aspect, float near, float far) const {
        Mat4 result = {0};

        float f = 1.0f / tanf(fov / 2.0f);

        result.matrix[0][0] = f / aspect;
        result.matrix[1][1] = f;

        result.matrix[2][2] = (far + near) / (near - far);

        // IMPORTANT: reversed because mat4Set uses GL_TRUE
        result.matrix[3][2] = -1.0f;
        result.matrix[2][3] = (2.0f * far * near) / (near - far);

        return result;
    }

    Rectangle hitbox() const {
        return {
            position,
            1,
            2
        };
    }

    void gravity(const float gravity, const float deltaTime) {
        if (deltaTime > 0.1) {
            velocityY += gravity * 0.1;
            position.y -= velocityY * 0.1;
        } else {
            velocityY += gravity * deltaTime;
            position.y -= velocityY * deltaTime;
        };
    }
};

/* struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 texcoords;
}; */

class VertexBuffer {
public: 
    unsigned int id;

    VertexBuffer(const float* data, size_t size) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }
    void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    ~VertexBuffer() {
        glDeleteBuffers(1, &id);
    }
};

class VertexArray {
public:
    unsigned int id;

    VertexArray() {
        glGenVertexArrays(1, &id);
    }

    void bind() const { glBindVertexArray(id); }
    void unbind() const { glBindVertexArray(0); }

    void setAttribute(unsigned int index, int quant, size_t size, size_t offset) {
        bind();
        glVertexAttribPointer(index, quant, GL_FLOAT, GL_FALSE, size, (void*)offset);
        glEnableVertexAttribArray(index);
    }

    ~VertexArray() {
        glDeleteVertexArrays(1, &id);
    }
};

class ElementBuffer {
public:
    unsigned int id;

    ElementBuffer(const unsigned int* data, size_t size) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }
    void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    ~ElementBuffer() {
        glDeleteBuffers(1, &id);
    }
};

class Shader {
public: 
    unsigned int vertexShader, fragmentShader, shaderProgram;

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

    void createProgram(unsigned int& program, unsigned int shader[2]) {
        program = glCreateProgram();
        glAttachShader(program, shader[0]);
        glAttachShader(program, shader[1]);
        glLinkProgram(program);
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        } 
    }
    
    Shader(const char* vertexShaderSource, const char* fragmentShaderSource) {
        createShader(vertexShader, vertexShaderSource, GL_VERTEX_SHADER);
        createShader(fragmentShader, fragmentShaderSource, GL_FRAGMENT_SHADER);
        unsigned int shaders[2] = {vertexShader, fragmentShader};
        createProgram(shaderProgram, shaders);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
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

    void floatSet(const char* name, const float x) {
        unsigned int location = glGetUniformLocation(shaderProgram, name);
        glUniform1f(location, x);
    }

    ~Shader() {
        glDeleteProgram(shaderProgram);
    }
};

class Texture {
public: 
    unsigned int id;
    int width, height, nrChannels;

    Texture(const char* filename) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

        if (data) {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        } else {
            std::cout << "failed";
        }
    }

    void bind() const {
        glBindTexture(GL_TEXTURE_2D, id);
    }
};

class Mesh {
public:
    VertexArray vao;
    VertexBuffer vbo;
    ElementBuffer ebo;
    int indexCount;

    Mesh(const float* vertexData, size_t vertexSize, const unsigned int* indexData, size_t indexSize, int indexCount)
        : vbo(vertexData, vertexSize), ebo(indexData, indexSize), indexCount(indexCount) {
        vao.bind();
        vbo.bind();
        ebo.bind();


        // x, y, z
        vao.setAttribute(0, 3, 8 * sizeof(float), 0);
        // u, v
        vao.setAttribute(1, 2, 8 * sizeof(float), 3 * sizeof(float));
        // nx, ny, nz
        vao.setAttribute(2, 3, 8 * sizeof(float), 5 * sizeof(float));

        vao.unbind();
    }

    void draw() const {
        vao.bind();
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }
};
