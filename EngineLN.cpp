#include <_time.h>
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION

#include "EngineLN.h"

bool EngineLN::App::init()
{
    if (!glfwInit())
    {
        printf("INFO: glfwInit() failed");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

    if (!window) {
        printf("INFO: glfwCreateWindow() failed");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("INFO: gladLoadGLLoader() failed");
        glfwTerminate();
        return false;
    }

    if (enet_initialize() != 0)
    {
        printf("INFO: ENet initialisation failed");
        return false;
    }

    glfwSwapInterval(0);

    glEnable(GL_DEPTH_TEST);

    sceneVertexShader = readShader("shaders/lighting.vert");
    sceneFragmentShader = readShader("shaders/lighting.frag");
    sceneShader.init(sceneVertexShader.c_str(), sceneFragmentShader.c_str());

    skyboxVertexShader = readShader("shaders/textured_skybox.vert");
    skyboxFragmentShader = readShader("shaders/textured_skybox.frag");
    skyboxShader.init(skyboxVertexShader.c_str(), skyboxFragmentShader.c_str());

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, mouseCallback);

    meshes.emplace_back();
    meshes.back().init("assets/sponza.obj", "textures/stone_brick.png", true);

    skybox.init(true);

    return true;
}

void EngineLN::App::run()
{
    MeshesBeginDrawing(sceneShader, camera, meshes, true);
    SkyboxBeginDrawing(skyboxShader, camera, skybox, true);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        MeshesDraw(sceneShader, camera, meshes, true);
        SkyboxDraw(skyboxShader, camera, skybox, true);

        keyInput(window, camera, 0.01f);

        network.update(camera.position);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void EngineLN::App::shutdown()
{
    network.shutdown();

    enet_deinitialize();

    glfwDestroyWindow(window);
    glfwTerminate();
}

bool EngineLN::App::startServer()
{
    return network.initServer();
}

bool EngineLN::App::joinServer(const char* ip)
{
    return network.initClient(ip);
}

void EngineLN::Mesh::init(const char* objFilePath,
    const char* textureFilePath,
    bool isTextured)
{
    readOBJ(objFilePath, vertices, indices);

    if (isTextured)
    {
        glGenTextures(1, &TextureID);
    }

    this->isTextured = isTextured;
    this->textureFilePath = textureFilePath;
}

void EngineLN::Mesh::sendToGPU()
{
    // VAO creation and binding
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // VBO creation and binding
    glGenBuffers(1, &VertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);

    // EBO creation and binding
    glGenBuffers(1, &ElementBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferID);

    // sending vertex data to GPU via VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    // sending attribute pointer to GPU via VAO
    // This attribute accounts for the first three values which are the x, y and z values of a vertex (3D coords)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    // layout = 0
    glEnableVertexAttribArray(0);
    // This attribute accounts for the third and fourth values which are u and v values of a vertex (2D texture coords)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));
    // layout = 1
    glEnableVertexAttribArray(1);
    // This attribute accounts for the fifth, sixth and seventh values which are crossed x, y and z values of a vertex (3D normal vectors)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
    // layout = 2
    glEnableVertexAttribArray(2);
    // sending index data to the GPU via EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    if (isTextured)
    {
        // OpenGL Texture creation and binding
        glBindTexture(GL_TEXTURE_2D, TextureID);

        // Sending texture data to GPU via OpenGL Texture
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char* data = stbi_load(textureFilePath, &TextureWidth, &TextureHeight, &TextureChannelCount, 0);

        if (data) {
            GLenum format = (TextureChannelCount == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, TextureWidth, TextureHeight, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        } else {
            std::cout << "failed";
        }

        // unbinding OpenGL Texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EngineLN::Mesh::draw() {
    // bind VAO (necessary for glDrawElements)
    glBindVertexArray(VertexArrayID);

    if (isTextured)
    {
        // bind OpenGL Texture (necessary for glDrawElements)
        glBindTexture(GL_TEXTURE_2D, TextureID);
    }

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    if (isTextured)
    {
        // unbind Opengl Texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // unbind VAO
    glBindVertexArray(0);
}

void EngineLN::Skybox::init(bool isTextured)
{
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

    if (isTextured)
    {
        glGenTextures(1, &TextureID);
    }
}

void EngineLN::Skybox::sendToGPU(bool isTextured)
{
    // VAO creation and binding
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // VBO creation and binding
    glGenBuffers(1, &VertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);

    // Sending vertex data to GPU via VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    // Sending attribute pointer to GPU via VAO.
    // This attribute accounts for the first three values which are the x, y and z values of a vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    // layout = 0
    glEnableVertexAttribArray(0);

    if (isTextured)
    {
        // OpenGL Texture binding
        glBindTexture(GL_TEXTURE_2D, TextureID);

        // Sending texture data to GPU via OpenGL Texture
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        for (int i = 0; i < 6; i++) {
            unsigned char* data = stbi_load(SkyboxFaces[i], &CubeMapWidth, &CubeMapHeight, &CubeMapChannelCount, 0);
            if (data) {
                GLenum format = (CubeMapChannelCount == 4) ? GL_RGBA : GL_RGB;
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, CubeMapWidth, CubeMapHeight, 0, format, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            } else {
                std::cout << SkyboxFaces[i];
            }
        }

        // unbinding OpenGL Texture
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    // unbinding VBO
    glBindVertexArray(0);
    // unbinding VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

glm::vec3 EngineLN::Camera::getForward() const
{
    return {
        cosf(pitch) * sinf(yaw),
        sinf(pitch),
        -cosf(pitch) * cosf(yaw)
    };
}

glm::vec3 EngineLN::Camera::getRight() const
{
    return {
        cosf(yaw),
        0.0f,
        sinf(yaw)
    };
}

glm::vec3 EngineLN::Camera::getUp() const
{
    return glm::normalize(glm::cross(getRight(), getForward()));
}

glm::mat4 EngineLN::Camera::view() const
{
    return glm::lookAt(
        position,
        position + getForward(),
        getUp()
    );
}

glm::mat4 EngineLN::Camera::perspective() const
{
    return glm::perspective(
        glm::radians(fov),
        aspect,
        near,
        far
    );
}

void EngineLN::Shader::createShader(unsigned int& shader, const char* shaderSource, GLenum type)
{
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

void EngineLN::Shader::createProgram(unsigned int& program, std::vector<unsigned int> shader)
{
    program = glCreateProgram();
    for (int i = 0; i < shader.size(); i++)
    {
        glAttachShader(program, shader[i]);
    }
    glLinkProgram(program);

    glLinkProgram(program);
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

void EngineLN::Shader::init(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    createShader(vertexShader, vertexShaderSource, GL_VERTEX_SHADER);
    createShader(fragmentShader, fragmentShaderSource, GL_FRAGMENT_SHADER);
    std::vector<unsigned int> shaders{vertexShader, fragmentShader};
    createProgram(shaderProgram, shaders);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void EngineLN::Shader::use() const
{
    glUseProgram(shaderProgram);
}

void EngineLN::Shader::mat4Set(const char* name, const glm::mat4 matrix) const
{
    unsigned int location = glGetUniformLocation(shaderProgram, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void EngineLN::Shader::vec4Set(const char* name, const glm::vec4 vector) const
{
    unsigned int location = glGetUniformLocation(shaderProgram, name);
    glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void EngineLN::Shader::vec3Set(const char* name, const glm::vec3 vector) const
{
    unsigned int location = glGetUniformLocation(shaderProgram, name);
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void EngineLN::Shader::floatSet(const char* name, const float x) const
{
    unsigned int location = glGetUniformLocation(shaderProgram, name);
    glUniform1f(location, x);
}

void EngineLN::Shader::intSet(const char* name, const int x) const
{
    unsigned int location = glGetUniformLocation(shaderProgram, name);
    glUniform1i(location, x);
}

EngineLN::Shader::~Shader() {
    glDeleteProgram(shaderProgram);
}

void EngineLN::PlayerManager::createPlayer(uint32_t id)
{
    players.emplace_back();
    players.back().id = id;
}

void EngineLN::PlayerManager::removePlayer(uint32_t id)
{
    players.erase(std::remove_if(players.begin(), players.end(), [id](const Player& player){
        return player.id == id;
    }), players.end());
}

EngineLN::Player* EngineLN::PlayerManager::getPlayer(uint32_t id)
{
    for (auto& player : players)
        if (player.id == id)
            return &player;

    return nullptr;
}

bool EngineLN::Network::initClient(const char* ip)
{
    host = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!host) return false;

    ENetAddress address{};
    enet_address_set_host(&address, ip);
    address.port = 7777;

    server = enet_host_connect(host, &address, 2, 0);
    if (!server) return false;

    isServer = false;

    return server != nullptr;
}

bool EngineLN::Network::initServer()
{
    ENetAddress address{};
    address.host = ENET_HOST_ANY;
    address.port = 7777;

    host = enet_host_create(&address, 8, 2, 0, 0);
    if (!host) return false;

    isServer = true;
    localID = 1;

    return host != nullptr;
}

void EngineLN::Network::update(const glm::vec3& localPosition)
{
    if (!host) return;

    ENetEvent event;

    while (enet_host_service(host, &event, 0) > 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
            {
                uint32_t id = nextID++;
                peerIDs[event.peer] = id;

                PlayerPacket connect{};
                connect.type = PLAYER_CONNECT;
                connect.id = id;

                ENetPacket* ConnectPacket = enet_packet_create(
                    &connect,
                    sizeof(connect),
                    ENET_PACKET_FLAG_RELIABLE
                );

                enet_peer_send(event.peer, 0, ConnectPacket);

                for (const auto& [playerID, position] : playerPositions)
                {
                    PlayerPacket state{};
                    state.type = PLAYER_POSITION;
                    state.id = playerID;
                    state.x = position.x;
                    state.y = position.y;
                    state.z = position.z;

                    ENetPacket* PositionPacket = enet_packet_create(
                        &state,
                        sizeof(state),
                        ENET_PACKET_FLAG_RELIABLE
                    );

                    enet_peer_send(event.peer, 0, PositionPacket);
                }

                break;
            }

            case ENET_EVENT_TYPE_RECEIVE:
            {
                if (event.packet->dataLength == sizeof(PlayerPacket))
                {
                    PlayerPacket packet{};
                    memcpy(&packet, event.packet->data, sizeof(packet));

                    if (isServer)
                    {
                        auto peer = peerIDs.find(event.peer);

                        if (peer != peerIDs.end() && packet.type == PLAYER_POSITION)
                        {
                            packet.id = peer->second;

                            playerPositions[packet.id] = {
                                packet.x,
                                packet.y,
                                packet.z
                            };

                            ENetPacket* broadcast = enet_packet_create(
                                &packet,
                                sizeof(packet),
                                0
                            );

                            enet_host_broadcast(host, 1, broadcast);
                        }
                    } else
                    {
                        if (packet.type == PLAYER_CONNECT)
                        {
                            localID = packet.id;
                        }
                        else if (packet.type == PLAYER_POSITION)
                        {
                            playerPositions[packet.id] = {
                                packet.x,
                                packet.y,
                                packet.z
                            };
                        }
                        else if (packet.type == PLAYER_DISCONNECT)
                        {
                            playerPositions.erase(packet.id);
                        }
                    }
                }

                enet_packet_destroy(event.packet);

                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT:
            {
                if (isServer)
                {
                    auto peer = peerIDs.find(event.peer);

                    if (peer != peerIDs.end())
                    {
                        uint32_t id = peer->second;

                        peerIDs.erase(peer);
                        playerPositions.erase(id);

                        PlayerPacket DisconnectPacket{};
                        DisconnectPacket.type = PLAYER_DISCONNECT;
                        DisconnectPacket.id = id;

                        ENetPacket* packet = enet_packet_create(
                            &DisconnectPacket,
                            sizeof(DisconnectPacket),
                            ENET_PACKET_FLAG_RELIABLE
                        );

                        enet_host_broadcast(host, 0, packet);
                    }
                } else
                {
                    server = nullptr;
                    localID = 0;
                    playerPositions.clear();
                }

                break;
            }

            default:
                break;
        }
    }

    auto now = std::chrono::steady_clock::now();

    if (now - lastSend >= std::chrono::milliseconds(50))
    {
        PlayerPacket PositionPacket{};
        PositionPacket.type = PLAYER_POSITION;
        PositionPacket.id = localID;
        PositionPacket.x = localPosition.x;
        PositionPacket.y = localPosition.y;
        PositionPacket.z = localPosition.z;

        if (isServer)
        {
            ENetPacket* packet = enet_packet_create(
                &PositionPacket,
                sizeof(PositionPacket),
                0
            );

            enet_host_broadcast(host, 1, packet);
        } else if (server && localID != 0)
        {
            ENetPacket* packet = enet_packet_create(
                &PositionPacket,
                sizeof(PositionPacket),
                0
            );

            enet_peer_send(server, 1, packet);
        }

        lastSend = now;
    }

    enet_host_flush(host);
}

void EngineLN::Network::sendPosition(const PlayerPacket& packet)
{
    if (!host || !server) return;

    ENetPacket* data = enet_packet_create(&packet,
        sizeof(PlayerPacket),
        ENET_PACKET_FLAG_UNSEQUENCED
    );

    enet_peer_send(server, 0, data);
}

void EngineLN::Network::shutdown()
{
    if (host)
        enet_host_destroy(host);

    host = nullptr;
    server = nullptr;
}

void readOBJ(const char* path,
    std::vector<EngineLN::Vertex>& vertices,
    std::vector<unsigned int>& indices
)
{
    std::ifstream file(path);
    std::string line;

    vertices.clear();
    indices.clear();

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> normals;

    while(std::getline(file, line)) {
        std::stringstream stream(line);
        std::string type;
        stream >> type;

        EngineLN::Vertex vertex;
        glm::vec3 position, normal;
        glm::vec2 texcoord;

        if (type == "v") {
            stream >> position.x >> position.y >> position.z;
            positions.push_back(position);
        } else if (type == "vt") {
            stream >> texcoord.x >> texcoord.y;
            texcoord.y = 1.0f - texcoord.y;
            texcoords.push_back(texcoord);
        } else if (type == "vn") {
            stream >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (type == "f") {
            std::string token;

            while (stream >> token) {
                std::replace(token.begin(), token.end(), '/', ' ');
                std::stringstream stream(token);

                int vertexIndex, textureIndex, normalIndex;

                stream >> vertexIndex >> textureIndex >> normalIndex;

                vertex.position = positions[vertexIndex - 1];
                vertex.texcoord = texcoords[textureIndex - 1];
                vertex.normal = normals[normalIndex - 1];

                vertices.push_back(vertex);
                indices.push_back(static_cast<int>(vertices.size() - 1));
            }
        }
    }
}

std::string readShader(const char* path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Draw functions for objects (no texture)
void MeshesBeginDrawing(const EngineLN::Shader& shader,
    const EngineLN::Camera& camera,
    std::vector<EngineLN::Mesh>& meshes,
    bool isTextured
)
{
    shader.use();

    // camera stuff
    shader.mat4Set("perspective", camera.perspective());
    shader.mat4Set("model", glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)));

    for (auto& mesh : meshes) {
        // sending a mesh's vertex data and texture data (if textured) to the GPU
        mesh.sendToGPU();
    }
}

void MeshesDraw(const EngineLN::Shader& shader,
    const EngineLN::Camera& camera,
    std::vector<EngineLN::Mesh>& meshes,
    bool isTextured
)
{
    shader.use();

    // sending the view matrix and camera position to the GPU //
    shader.mat4Set("view", camera.view());
    shader.vec3Set("viewPos", camera.position);

    for (auto& mesh : meshes) {
        // drawing each mesh
        mesh.draw();
    }
}

// Draw functions for skybox
void SkyboxBeginDrawing(const EngineLN::Shader& shader,
    const EngineLN::Camera& camera,
    EngineLN::Skybox& skybox,
    bool isTextured
)
{
    shader.use();

    // camera stuff
    shader.mat4Set("perspective", camera.perspective());

    skybox.sendToGPU(isTextured);

    if (!isTextured)
    {
        shader.vec3Set("upperHemisphereColor", {0.45, 0.70, 0.90});
        shader.vec3Set("lowerHemisphereColor", {0.65, 0.67, 0.68});
    }
}

void SkyboxDraw(const EngineLN::Shader& shader,
    const EngineLN::Camera& camera,
    EngineLN::Skybox& skybox,
    bool isTextured
)
{
    glDepthFunc(GL_LEQUAL);

    shader.use();

    // sending the view matrix to the GPU //
    shader.mat4Set("view", camera.view());

    glBindVertexArray(skybox.VertexArrayID);

    if (isTextured)
    {
        glActiveTexture(GL_TEXTURE0);
    }

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}

void keyInput(GLFWwindow* window, EngineLN::Camera &camera, float speed)
{
    glm::vec3 forward = camera.getForward();
    glm::vec3 right = camera.getRight();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.position.x += speed * forward.x;
        camera.position.z += speed * forward.z;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.position.x -= speed * forward.x;
        camera.position.z -= speed * forward.z;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.position.x += speed * right.x;
        camera.position.z += speed * right.z;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.position.x -= speed * right.x;
        camera.position.z -= speed * right.z;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.position.y += speed;
    }

/*     if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.position.y -= speed;
    } */
}

void mouseInput(EngineLN::Camera &camera, double xpos, double ypos, float sensitivity)
{
    static bool firstMouse = false;
    static float lastX;
    static float lastY;

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

    camera.yaw += dx * sensitivity;
    camera.pitch -= dy * sensitivity;

    if (camera.pitch > degreesToRadians(89)) camera.pitch = degreesToRadians(89);
    if (camera.pitch < degreesToRadians(-89)) camera.pitch = degreesToRadians(-89);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    auto* app = static_cast<EngineLN::App*>(glfwGetWindowUserPointer(window));
    if (app)
        mouseInput(app->camera, xpos, ypos, 0.002f);
}

float degreesToRadians(float angle) { return angle * (M_PI / 180); }
