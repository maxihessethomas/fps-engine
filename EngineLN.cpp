#include "glm/ext/matrix_transform.hpp"
#include "include/Texture/Texture.h"
#define STB_IMAGE_IMPLEMENTATION

#include "EngineLN.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <glm/glm.hpp>

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

    glfwSwapInterval(0);

    glEnable(GL_DEPTH_TEST);

    sceneVertexShader = readShader("shaders/lighting.vert");
    sceneFragmentShader = readShader("shaders/lighting.frag");
    sceneShader.init(sceneVertexShader.c_str(), sceneFragmentShader.c_str());
    sceneShader.type = EngineLN::Shader::ShaderType::SCENE_SHADER;

    skyboxVertexShader = readShader("shaders/textured_skybox.vert");
    skyboxFragmentShader = readShader("shaders/textured_skybox.frag");
    skyboxShader.init(skyboxVertexShader.c_str(), skyboxFragmentShader.c_str());
    skyboxShader.type = EngineLN::Shader::ShaderType::SKYBOX_SHADER;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, mouseCallback);

/*     Object level("assets/level.obj", glm::vec4(1.0, 0.451, 0.0, 1.0)); */

/*     objects.push_back(level); */

    skybox.init();

    return true;
}

void EngineLN::App::run()
{
    ObjectsBeginDrawing(sceneShader, camera, objects);
    SkyboxBeginDrawing(skyboxShader, camera, skybox, true);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ObjectsDraw(sceneShader, camera, objects);
        SkyboxDraw(skyboxShader, camera, skybox, true);

        keyInput(window, camera, 0.01f);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void EngineLN::App::shutdown()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void EngineLN::Mesh::init(const char* objFilePath)
{
    readOBJ(objFilePath, vertices, indices);
}

void EngineLN::Mesh::sendToGPU()
{
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glGenBuffers(1, &VertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ElementBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EngineLN::Skybox::init()
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
    // Sending attribute pointer to GPU via VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    if (isTextured)
    {
        // Texture creation and binding
        glGenTextures(1, &TextureID);
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
        glBindTexture(GL_TEXTURE_2D, 0);
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

/* EngineLN::Texture::Texture()
{
    glGenTextures(1, &id);
}

void EngineLN::Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
} */

/* void EngineLN::Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void EngineLN::Texture::init(const char* path)
{
    bind();

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        std::cout << "failed";
    }
    unbind();
} */

EngineLN::Object::Object(const char* objFilePath,
    glm::vec4 color)
{
    mesh.init(objFilePath);
    this->color = color;
}

void EngineLN::Object::sendToGPU()
{
    mesh.sendToGPU();
}

void EngineLN::Object::draw() {
    glBindVertexArray(mesh.VertexArrayID);

    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
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
void ObjectsBeginDrawing(const EngineLN::Shader& shader,
    const EngineLN::Camera& camera,
    std::vector<EngineLN::Object>& objects
)
{
    shader.use();

    // camera stuff
    shader.mat4Set("perspective", camera.perspective());
    shader.mat4Set("model", glm::scale(glm::mat4(1.0f), glm::vec3(10, 10, 10)));

    for (auto& object : objects) {
        // sending an object's vertex data to the GPU
        object.sendToGPU();

    }
}

void ObjectsDraw(const EngineLN::Shader& shader,
    const EngineLN::Camera& camera,
    std::vector<EngineLN::Object>& objects
)
{
    shader.use();

    // sending the view matrix and camera position to the GPU //
    shader.mat4Set("view", camera.view());
    shader.vec3Set("viewPos", camera.position);

    for (auto& object : objects) {

        // sending an objects' color data to the gpu
        shader.vec4Set("color", object.color);

        object.draw();
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

    if (isTextured)
    {
        skybox.sendToGPU(true);
    } else
    {
        shader.vec3Set("upperHemisphereColor", {0.45, 0.70, 0.90});
        shader.vec3Set("lowerHemisphereColor", {0.65, 0.67, 0.68});

        skybox.sendToGPU(false);
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

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.position.y -= speed;
    }
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


/* void beginDrawing(const EngineLN::Shader& shader, const EngineLN::Camera& camera)
{
    shader.use();
    shader.mat4Set("projection", camera.perspective());
    shader.mat4Set("view", camera.view());
} */


/* void draw(const EngineLN::Mesh& mesh, const EngineLN::Texture& texture, const EngineLN::Shader shader)
{
    shader.mat4Set("model", glm::mat4(1.0f));
    glBindTexture(1, &mesh.);

} */

/*     glGenTextures(1, &TextureID);
glBindTexture(GL_TEXTURE, TextureID);

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
    printf("INFO: texture loading failed");
}

glBindTexture(GL_TEXTURE, 0);
 */
