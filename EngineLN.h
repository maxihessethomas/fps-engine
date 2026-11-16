#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <cstdint>
#include <iostream>
#include <sstream>
#include <fstream>

#include <glm/glm.hpp>

namespace EngineLN {

    class Camera {
    public:

        glm::vec3 position;
        float yaw{}, pitch{}, roll{}, fov = 60, near = 0.1f, far = 100.0f;
        float aspect = 800.0f / 600.0f;

        glm::vec3 getForward() const;
        glm::vec3 getRight() const;
        glm::vec3 getUp() const;

        glm::mat4 view() const;
        glm::mat4 perspective() const;

    };

    class Shader
    {
    public:

        unsigned int vertexShader, fragmentShader, shaderProgram;

        void createShader(unsigned int&, const char*, GLenum);
        void createProgram(unsigned int&, std::vector<unsigned int>);

        void init(const char*, const char*);

        void use() const;

        void mat4Set(const char*, const glm::mat4) const;
        void vec3Set(const char*, const glm::vec3) const;
        void floatSet(const char*, const float) const;
        void intSet(const char*, const int) const;

        ~Shader();
    };

    class Texture
    {
    public:

        unsigned int id;
        int width, height, nrChannels;

        Texture();

        void bind() const;
        void unbind() const;

        void init(const char*);

    };

    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 texcoord;
        glm::vec3 normal;
    };

    class Mesh
    {
    public:
        GLenum drawMode;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        Mesh(const char*);
    private:
        unsigned int VertexBufferID, VertexArrayID, ElementBufferID, TextureID;
    };

    struct Item
    {
        std::string name;
        Mesh mesh;
        Texture texture;
    };

    struct Inventory
    {
        std::array<Item, 4> items;
        int activeItem = 0;
    };

    struct Entity
    {
        std::string name;
        glm::vec3 position;
        Mesh mesh;
        Texture texture;
    };

    class App
    {
    public:
        std::string name = "EngineLN";
        uint32_t width = 800;
        uint32_t height = 600;

        bool init();
        void run();
        void shutdown();

    private:
        GLFWwindow* window;

        Camera camera;

        std::string vertexShaderSource, fragmentShaderSource;
        Shader shader;
    };

}

void
readOBJ(const char*,
    std::vector<EngineLN::Vertex>&,
    std::vector<unsigned int>&
);

std::string
readShader(const char*);

void
beginDrawing(const EngineLN::Shader&, const EngineLN::Camera&);

void
draw(const EngineLN::Mesh&, const EngineLN::Texture&, const EngineLN::Shader);
