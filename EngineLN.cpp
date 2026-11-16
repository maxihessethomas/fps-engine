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
        return true;
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

    vertexShaderSource = readShader("shaders/lighting.vert");
    fragmentShaderSource = readShader("shaders/lighting.frag");
    shader.init(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

    return true;
}

void EngineLN::App::run()
{
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void EngineLN::App::shutdown()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

EngineLN::Mesh::Mesh(const char* objFilePath)
{
    readOBJ(objFilePath, vertices, indices);

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
    glm::vec3 forward = getForward();
    glm::vec3 right = getRight();
    glm::vec3 up = getUp();

    return {
        right.x, right.y, right.z, 0,
        up.x, up.y, up.z, 0,
        -forward.x, -forward.y, -forward.z, 0,
        0, 0, 0, 1
    };
}

glm::mat4 EngineLN::Camera::perspective() const
{
    float f = 1.0f / tanf(fov / 2.0f);

    return {
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (far+near) / (near-far), (2.0f*far*near) / (near-far),
        0, 0, -1.0f, 1,
    };
}

void EngineLN::Shader::createShader(unsigned int& shader, const char* shaderSource, GLenum type)
{
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
}

void EngineLN::Shader::createProgram(unsigned int& program, std::vector<unsigned int> shader)
{
    program = glCreateProgram();
    for (int i = 0; i < shader.size(); i++)
    {
        glAttachShader(program, shader[i]);
    }
    glLinkProgram(program);
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
    glUniformMatrix4fv(location, 1, GL_TRUE, &matrix[0][0]);
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

EngineLN::Texture::Texture()
{
    glGenTextures(1, &id);
}

void EngineLN::Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void EngineLN::Texture::unbind() const
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

void beginDrawing(const EngineLN::Shader& shader, const EngineLN::Camera& camera)
{
    shader.use();
    shader.mat4Set("projection", camera.perspective());
    shader.mat4Set("view", camera.view());
}


void draw(const EngineLN::Mesh& mesh, const EngineLN::Texture& texture, const EngineLN::Shader shader)
{
    shader.mat4Set("model", glm::mat4(1.0f));
    glBindTexture(1, &mesh.);

}

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
