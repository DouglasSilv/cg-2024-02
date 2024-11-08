#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <glm/glm.hpp>

class Material {
public:
    Material();
    Material(const std::string& diffuseTexturePath, const glm::vec3& Ka, const glm::vec3& Kd, const glm::vec3& Ks);
    
    void SetName(const std::string& name);
    void SetKa(const glm::vec3& Ka);
    void SetKd(const glm::vec3& Kd);
    void SetKs(const glm::vec3& Ks);
    void SetDiffuseTexture(const std::string& texturePath);
    
    const std::string& GetName() const;
    const glm::vec3& GetKa() const;
    const glm::vec3& GetKd() const;
    const glm::vec3& GetKs() const;
    const std::string& GetDiffuseTexture() const;
    GLuint GetTextureID() const;

    unsigned int textureID;  // ID da textura no OpenGL

private:
    std::string name;
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    std::string diffuseTexturePath;
};

#endif
