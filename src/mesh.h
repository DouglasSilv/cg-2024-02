#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "materialproperties.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
    glm::vec3 Normal;
};

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, MaterialProperties material);
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    MaterialProperties material;

    void Draw();

private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};
