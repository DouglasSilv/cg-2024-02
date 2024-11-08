// model.cpp
#include "model.h"
#include "materialproperties.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stb_image.h>
#include <unordered_map>

Model::Model(const std::string& path) {
    directory = path.substr(0, path.find_last_of('/'));
    loadModel(path);
}

void Model::Draw(Shader& shader) {
    for (Mesh& mesh : meshes) {
        shader.setVec3("kd", mesh.material.Kd);
        shader.setVec3("ka", mesh.material.Ka);
        shader.setVec3("ks", mesh.material.Ks);
        shader.setInt("texBuffer", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.material.diffuseTexture);
        mesh.Draw();
    }
}

void Model::loadModel(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to load model: " << path << std::endl;
        return;
    }

    std::string line;
    bool isReadingFace = false;
    static std::vector<unsigned int> indices;
    static std::vector<Vertex> vertices;
    while (std::getline(file, line)) {
        static std::vector<glm::vec3> tempPositions;
        static std::vector<glm::vec3> tempNormals;
        static std::vector<glm::vec2> tempTexCoords;

        std::istringstream ss(line);
        std::string identifier;
        ss >> identifier;
        
        if (isReadingFace && identifier != "f") {
            Mesh newMesh = Mesh(vertices, indices, materialTextures[currentMaterial]);
            meshes.push_back(newMesh);

            vertices.clear();
            indices.clear();
        }

        isReadingFace = false;
        if (identifier == "v") { // Vertex position
            glm::vec3 position;
            ss >> position.x >> position.y >> position.z;
            tempPositions.push_back(position);
        }
        else if (identifier == "vt") { // Vertex texture coordinate
            glm::vec2 texCoord;
            ss >> texCoord.x >> texCoord.y;
            tempTexCoords.push_back(texCoord);
        }
        else if (identifier == "vn") { // Vertex normal
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        }
        else if (identifier == "f") { // Face
            unsigned int vertexIndex[3], texCoordIndex[3], normalIndex[3];
            char slash; // to handle the slashes in the faces
            for (int i = 0; i < 3; i++) {
                ss >> vertexIndex[i] >> slash >> texCoordIndex[i] >> slash >> normalIndex[i];
                Vertex vertex;
                vertex.Position = tempPositions[vertexIndex[i] - 1];
                vertex.TexCoords = tempTexCoords[texCoordIndex[i] - 1];
                vertex.Normal = tempNormals[normalIndex[i] - 1];
                vertices.push_back(vertex);
                indices.push_back(vertices.size() - 1);
            }
            isReadingFace = true;
        } else if (identifier == "usemtl") { // Material name
            /*if (!vertices.empty()) {
                Mesh newMesh = Mesh(vertices, indices, materialTextures[currentMaterial]);
                meshes.push_back(newMesh);

                vertices.clear();
                indices.clear();
            }

            ss >> currentMaterial;*/
        } else if (identifier == "mtllib") { // Material library file
            std::string mtlFile;
            ss >> mtlFile;
            loadMaterial(mtlFile);
        }
    }

    if (!vertices.empty() && !indices.empty()) {
        Mesh newMesh = Mesh(vertices, indices, materialTextures[currentMaterial]);
        meshes.push_back(newMesh);

        vertices.clear();
        indices.clear();
    }
    file.close();
}

void Model::loadMaterial(const std::string& materialPath) {
    std::ifstream file(directory + '/' + materialPath);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de material: " << materialPath << std::endl;
        return;
    }

    std::string line;
    MaterialProperties materialProperties;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream sstream(line);
        std::string keyword;
        sstream >> keyword;

        if (keyword == "newmtl") {
            // Adiciona o material anterior ao mapa se houver um material em processamento
            if (!currentMaterial.empty()) {
                materialTextures[currentMaterial] = materialProperties;
            }

            // Atualiza o material atual
            sstream >> currentMaterial;
            materialProperties = MaterialProperties();  // Reinicia as propriedades para o novo material
        }
        else if (keyword == "Kd") {
            sstream >> materialProperties.Kd.r >> materialProperties.Kd.g >> materialProperties.Kd.b;
        }
        else if (keyword == "Ka") {
            sstream >> materialProperties.Ka.r >> materialProperties.Ka.g >> materialProperties.Ka.b;
        }
        else if (keyword == "Ks") {
            sstream >> materialProperties.Ks.r >> materialProperties.Ks.g >> materialProperties.Ks.b;
        }
        else if (keyword == "Ke") {
            sstream >> materialProperties.Ke.r >> materialProperties.Ke.g >> materialProperties.Ke.b;
        }
        else if (keyword == "map_Kd") {
            std::string texturePath;
            sstream >> texturePath;
            materialProperties.diffuseTexture = TextureFromFile(texturePath, directory);
        }
    }

    // Salva o último material processado
    if (!currentMaterial.empty()) {
        materialTextures[currentMaterial] = materialProperties;
    }

    file.close();
}


unsigned int Model::TextureFromFile(const std::string path, const std::string &directory) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    unsigned int textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
    
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    } else {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}
