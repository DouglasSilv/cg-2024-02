#include "model.h"
#include <fstream>
#include <sstream>
#include <iostream>

Model::Model(const std::string &path) {
    loadModel(path);
}

void Model::Draw(Shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void Model::loadModel(const std::string &path) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << path << std::endl;
        return;
    }

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec3> temp_normals;
    std::vector<glm::vec2> temp_texCoords;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            glm::vec3 position;
            iss >> position.x >> position.y >> position.z;
            temp_positions.push_back(position);
        } else if (prefix == "vn") {
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        } else if (prefix == "vt") {
            glm::vec2 texCoord;
            iss >> texCoord.x >> texCoord.y;
            temp_texCoords.push_back(texCoord);
        } else if (prefix == "f") {
            unsigned int vertexIndex[3], texCoordIndex[3], normalIndex[3];
            char slash; // to handle the slashes in the faces
            for (int i = 0; i < 3; i++) {
                iss >> vertexIndex[i] >> slash >> texCoordIndex[i] >> slash >> normalIndex[i];
                Vertex vertex;
                vertex.Position = temp_positions[vertexIndex[i] - 1];
                vertex.TexCoords = temp_texCoords[texCoordIndex[i] - 1];
                vertex.Normal = temp_normals[normalIndex[i] - 1];
                vertices.push_back(vertex);
                indices.push_back(vertices.size() - 1);
            }
        }
    }
    
    file.close();

    meshes.push_back(Mesh(vertices, indices, textures));
}
