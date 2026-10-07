#pragma once

#include "math.hpp"
#include "buffer.hpp"

class Cube {
public: 
    Vec3 position = {0.0f, 10.0f, 0.0f};
    float velocityY;
    VertexArray VAO;
    VertexBuffer VBO;
    ElementBuffer EBO;
    int indexCount;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    Cube() {
        float x = position.x; float y = position.y; float z = position.z;

        vertices = {
            x, y, z,               
            x + 1.0f, y, z,             
            x, y + 1.0f, z,                
            x + 1.0f, y + 1.0f, z,         

            x, y, z + 1.0f,             
            x + 1.0f, y, z + 1.0f,      
            x, y + 1.0f, z + 1.0f,        
            x + 1.0f, y + 1.0f, z + 1.0f
        };

        indices = {
            4, 5, 7,
            7, 6, 4,

            0, 2, 3,
            3, 1, 0,

            0, 4, 6,
            6, 2, 0,

            1, 3, 7,
            7, 5, 1,

            0, 1, 5,
            5, 4, 0,

            2, 6, 7,
            7, 3, 2,
        };

        indexCount = sizeof(indices) / sizeof(int);

                VAO.bind();

        VBO.bind();
        VBO.setData(vertices, sizeof(vertices));

        EBO.bind();
        EBO.setData(indices, sizeof(indices));

        VAO.setAttribute(0, 3, 3 * sizeof(float), 0, GL_FLOAT);
/*         VAO.setAttribute(1, 2, 8 * sizeof(float), 3 * sizeof(float), GL_FLOAT);
        VAO.setAttribute(2, 3, 8 * sizeof(float), 5 * sizeof(float), GL_FLOAT);
 */
        VAO.unbind();
    }

    void draw() const {
        VAO.bind();
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        VAO.unbind();
    }
};