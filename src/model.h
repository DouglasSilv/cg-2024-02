// model.h
#pragma once
#include <string>
#include <vector>
#include "mesh.h"
#include "shader.h"
#include "materialproperties.h"
#include <unordered_map>

class Model {
public:
    Shader shader;
    Model(const std::string& path);
    void Draw();

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::unordered_map<std::string, MaterialProperties> materialTextures;
    std::string currentMaterial;
    std::vector<unsigned int> indices;
    std::vector<Vertex> vertices;
    std::vector<glm::vec3> tempPositions;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempTexCoords;

    void loadModel(const std::string& path);
    unsigned int TextureFromFile(const std::string path, const std::string &directory);
    void loadMaterial(const std::string& filename);
};
