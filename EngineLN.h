#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <cstdint>
#include <iostream>
#include <sstream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace EngineLN {

    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 texcoord;
        glm::vec3 normal;
    };

    class Camera {
    public:

        glm::vec3 position = {0, 0, 3};
        float yaw{}, pitch{}, roll{}, fov = 60, near = 0.1f, far = 1000.0f;
        float aspect = 1600.0f / 1200.0f;

        glm::vec3 getForward() const;
        glm::vec3 getRight() const;
        glm::vec3 getUp() const;

        glm::mat4 view() const;
        glm::mat4 perspective() const;

    };

    class Shader
    {
    public:

        enum class ShaderType {
            SCENE_SHADER,
            SKYBOX_SHADER
        };

        ShaderType type;

        unsigned int vertexShader, fragmentShader, shaderProgram;

        void createShader(unsigned int&, const char*, GLenum);
        void createProgram(unsigned int&, std::vector<unsigned int>);

        void init(const char*, const char*);

        void use() const;

        void mat4Set(const char*, const glm::mat4) const;
        void vec4Set(const char*, const glm::vec4) const;
        void vec3Set(const char*, const glm::vec3) const;
        void floatSet(const char*, const float) const;
        void intSet(const char*, const int) const;

        ~Shader();

    };

/*     class Texture
    {
    public:

        unsigned int id;
        int width, height, nrChannels;

        Texture();

        void bind() const;
        void unbind() const;

        void init(const char*);

    }; */

    class Mesh
    {
    public:
        unsigned int VertexArrayID;

        GLenum drawMode;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        void init(const char*);
        void sendToGPU();

    private:
        unsigned int VertexBufferID, ElementBufferID, TextureID;
    };

    class Skybox
    {
    public:
        unsigned int VertexArrayID;

        std::vector<float> vertices;

        void init();
        void sendToGPU();

    private:
        unsigned int VertexBufferID, ElementBufferID;
    };

    class Object {
    public:
        bool handheld;

        // actual mesh and texture of the object //
        Mesh mesh;
        glm::vec4 color;

        Skybox skybox;

        // lighting stuff
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;

        Object(const char*, glm::vec4);

        void sendToGPU();
        void draw();

    };

/*     struct Item
    {
        std::string weapons{
            "assets/shotgun.obj"
        };
        std::string name;
        Mesh mesh;
    }; */

/*     struct Entity
    {
        std::string name;
        glm::vec3 position;
        Mesh mesh;
    }; */

/*     struct Inventory
    {
        std::array<Item, 4> items;
        int activeItem = 0;
    }; */

    class App
    {
    public:

        // needs to be public cuz of mouseCallback //
        Camera camera;

        std::vector<Object> objects;
        Skybox skybox;

        std::string name = "EngineLN";
        uint32_t width = 1600;
        uint32_t height = 1200;

        bool init();
        void run();
        void shutdown();

    private:
        GLFWwindow* window;


        std::string sceneVertexShader, sceneFragmentShader;
        Shader sceneShader;

        std::string skyboxVertexShader, skyboxFragmentShader;
        Shader skyboxShader;
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
beginDrawing(const EngineLN::Shader&,
    const EngineLN::Camera&,
    std::vector<EngineLN::Object>&
);

void
beginDrawing(const EngineLN::Shader&,
    const EngineLN::Camera&,
    EngineLN::Skybox&
);

void
draw(const EngineLN::Shader&,
    const EngineLN::Camera&,
    std::vector<EngineLN::Object>&
);

void
draw(const EngineLN::Shader&,
    const EngineLN::Camera&,
    EngineLN::Skybox&
);

void
keyInput(GLFWwindow*, EngineLN::Camera&, float);

void
mouseInput(EngineLN::Camera&, double, double, float);

void mouseCallback(GLFWwindow*, double, double);

float
degreesToRadians(float);
