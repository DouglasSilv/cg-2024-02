#include "obj_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

void loadObj(const std::string& path, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo OBJ!" << std::endl;
        return;
    }

    std::string line;
    std::vector<glm::vec3> positions;
    std::vector<glm::ivec3> faces;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string keyword;
        ss >> keyword;

        if (keyword == "v") {
            glm::vec3 position;
            ss >> position.x >> position.y >> position.z;
            positions.push_back(position);
        } else if (keyword == "f") {
            glm::ivec3 face;
            ss >> face.x >> face.y >> face.z;
            face -= glm::ivec3(1, 1, 1);  // OBJ usa índice 1, ajustamos para índice 0
            faces.push_back(face);
        }
    }

    for (const auto& face : faces) {
        for (int i = 0; i < 3; ++i) {
            Vertex vertex;
            vertex.Position = positions[face[i]];
            vertices.push_back(vertex);
            indices.push_back(indices.size());
        }
    }

    file.close();
}
