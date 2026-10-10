#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

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

    std::ifstream file("data.txt");
    std::string line;
    std::stringstream stream;

    while(std::getline(file, line)) {
        std::stringstream stream(line);
        std::string type;
        stream >> type;
        std::cout << type;
    }
}