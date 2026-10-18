#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

#include "vertex.h"
#include "import.hpp"
#include "math.hpp"

std::string readShader(const char* path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void readOBJ(const char* path, 
    std::vector<Vertex>& vertices,
    std::vector<unsigned int>& indices) {

    std::ifstream file(path);
    std::string line;

    vertices.clear();
    indices.clear();

    std::vector<Vec3> positions;
    std::vector<Vec2> texcoords;
    std::vector<Vec3> normals;

    while(std::getline(file, line)) {
        std::stringstream stream(line);
        std::string type;
        stream >> type;
        
        Vertex vertex;
        Vec3 position, normal;
        Vec2 texcoord;

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