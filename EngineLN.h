#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <cstdint>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <cstring>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

extern "C" {
    #include <enet/enet.h>
}

float
degreesToRadians(float);

namespace EngineLN {

    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 texcoord;
        glm::vec3 normal;
    };

    class Camera {
    public:

        glm::vec3 position = {0, 10, 0};
        float yaw{degreesToRadians(90)}, pitch{degreesToRadians(30)}, roll{}, fov = 110, near = 0.1f, far = 1000.0f;
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

    class Mesh
    {
    public:

        unsigned int VertexArrayID, TextureID;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        bool isTextured;

        const char* textureFilePath;

        // lighting stuff
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;

        void init(const char*, const char*, bool);
        void sendToGPU();
        void draw();

    private:
        unsigned int VertexBufferID, ElementBufferID;

        int TextureWidth, TextureHeight, TextureChannelCount;
    };

    class Skybox
    {
    public:

        unsigned int VertexArrayID, TextureID;

        std::vector<float> vertices;

        const char* SkyboxFaces[6] =
        {
            "textures/skybox/right.jpg",
            "textures/skybox/left.jpg",
            "textures/skybox/top.jpg",
            "textures/skybox/bottom.jpg",
            "textures/skybox/front.jpg",
            "textures/skybox/back.jpg"
        };

        void init(bool);
        void sendToGPU(bool);

    private:
        unsigned int VertexBufferID, ElementBufferID;

        int CubeMapWidth, CubeMapHeight, CubeMapChannelCount;
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

    class Player
    {
    public:
        uint32_t id;

        Camera camera;

        glm::vec3 position{0.0f};
        glm::vec3 rotation{0.0f};

        bool isLocalPlayer = false;

        bool isGrounded = false;
        bool jump = false;

        glm::vec3 velocity{0.0f};

        float groundedAcceleration = 0.1f;
        float airAcceleration = 0.02f;

        float groundedFriction = 0.06f;
        float stopSpeed = 0.02f;

        float groundedMaximumSpeed = 0.08f;
        float airMaximumSpeed = 0.08f;

        float jumpSpeed = 0.08f;
        float gravity = 0.00002f;

        void update(const glm::vec3& wishDirection);

    private:
        void accelerate(const glm::vec3& wishDirection,
            float wishSpeed,
            float acceleration
        );

        void applyFriction();
    };

    class PlayerManager
    {
    private:
        std::vector<Player> players;

        void createPlayer(uint32_t id);
        void removePlayer(uint32_t id);
        Player* getPlayer(uint32_t id);
    };

    enum PacketType : uint8_t
    {
        PLAYER_CONNECT = 1,
        PLAYER_POSITION = 2,
        PLAYER_DISCONNECT = 3
    };

    #pragma pack(push, 1)
    struct PlayerPacket
    {
        uint32_t id;
        PacketType type;
        float x, y, z, pitch, yaw;
    };
    #pragma pack(pop)

    class Network
    {
    public:
        ENetHost* host = nullptr;
        ENetPeer* server = nullptr;

        bool isServer = false;
        uint32_t localID = 0;
        uint32_t nextID = 2;

        std::unordered_map<ENetPeer*, uint32_t> peerIDs;
        std::unordered_map<uint32_t, glm::vec3> playerPositions;
        std::chrono::steady_clock::time_point lastSend;

        bool initClient(const char* ip);
        bool initServer();

        void update(const glm::vec3& localPosition);
        void sendPosition(const PlayerPacket& packet);

        void shutdown();
    };

    class App
    {
    public:

        // needs to be public cuz of mouseCallback //
        Player player;
        Skybox skybox;

        // networking stuff
        PlayerManager playerManager;

        std::vector<Mesh> meshes;
/*         std::vector<Texture> textures; */

        std::string name = "EngineLN";
        uint32_t width = 1600;
        uint32_t height = 1200;

        bool init();
        void run();
        void shutdown();

        bool startServer();
        bool joinServer(const char*);

    private:
        GLFWwindow* window;

        std::string sceneVertexShader, sceneFragmentShader;
        Shader sceneShader;

        std::string skyboxVertexShader, skyboxFragmentShader;
        Shader skyboxShader;

        Network network;
    };
}

void
readOBJ(const char*,
    std::vector<EngineLN::Vertex>&,
    std::vector<unsigned int>&
);

std::string
readShader(const char*);

// Draw functions for objects (no texture)
void
MeshesBeginDrawing(const EngineLN::Shader&,
    const EngineLN::Player&,
    std::vector<EngineLN::Mesh>&,
    bool
);

void
MeshesDraw(const EngineLN::Shader&,
    const EngineLN::Player&,
    std::vector<EngineLN::Mesh>&,
    bool
);

// Draw functions for skybox
void
SkyboxBeginDrawing(const EngineLN::Shader&,
    const EngineLN::Player&,
    EngineLN::Skybox&,
    bool
);

void
SkyboxDraw(const EngineLN::Shader&,
    const EngineLN::Player&,
    EngineLN::Skybox&,
    bool
);

void
keyInput(GLFWwindow*, EngineLN::Player&);

void
mouseInput(EngineLN::Camera&, double, double, float);

void
mouseCallback(GLFWwindow*, double, double);
