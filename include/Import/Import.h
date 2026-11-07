#pragma once

#include "Vertex/Vertex.h"

namespace EngineLN 
{

    std::string readShader(const char* path);

    void readOBJ(
        const char* path, 
        std::vector<Vertex>& vertices,
        std::vector<unsigned int>& indices);

}