#include "material.h"
#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

Material::Material()
    : Ka(glm::vec3(0.2f, 0.2f, 0.2f)), Kd(glm::vec3(0.8f, 0.8f, 0.8f)), Ks(glm::vec3(1.0f, 1.0f, 1.0f)), diffuseTexturePath("") {
    textureID = 0; // Inicializa o ID da textura como 0 (sem textura)
}

Material::Material(const std::string& diffuseTexturePath, const glm::vec3& Ka, const glm::vec3& Kd, const glm::vec3& Ks)
    : diffuseTexturePath(diffuseTexturePath), Ka(Ka), Kd(Kd), Ks(Ks) {
    textureID = 0;
}

void Material::SetName(const std::string& name) {
    this->name = name;
}

void Material::SetKa(const glm::vec3& Ka) {
    this->Ka = Ka;
}

void Material::SetKd(const glm::vec3& Kd) {
    this->Kd = Kd;
}

void Material::SetKs(const glm::vec3& Ks) {
    this->Ks = Ks;
}

void Material::SetDiffuseTexture(const std::string& texturePath) {
    this->diffuseTexturePath = texturePath;
    // Carregar a textura ao definir o caminho
    if (!texturePath.empty()) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "Erro ao carregar a textura: " << texturePath << std::endl;
        }
        stbi_image_free(data);
    }
}

const std::string& Material::GetName() const {
    return name;
}

const glm::vec3& Material::GetKa() const {
    return Ka;
}

const glm::vec3& Material::GetKd() const {
    return Kd;
}

const glm::vec3& Material::GetKs() const {
    return Ks;
}

const std::string& Material::GetDiffuseTexture() const {
    return diffuseTexturePath;
}

GLuint Material::GetTextureID() const {
    return textureID;
}
