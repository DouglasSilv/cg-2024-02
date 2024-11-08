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
    Model(const std::string& path);
    void Draw(Shader& shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::unordered_map<std::string, MaterialProperties> materialTextures;
    std::string currentMaterial;

    void loadModel(const std::string& path);
    unsigned int TextureFromFile(const std::string path, const std::string &directory);
    void loadMaterial(const std::string& filename);
};
