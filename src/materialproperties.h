#ifndef MATERIAL_PROPERTIES_H
#define MATERIAL_PROPERTIES_H

#include <glm/glm.hpp>
#include <string>

struct MaterialProperties {
    glm::vec3 Kd;
    glm::vec3 Ka; 
    glm::vec3 Ks; 
    glm::vec3 Ke;
    
    unsigned int diffuseTexture = 0; // ID da textura difusa

    MaterialProperties()
        : Kd(0.8f, 0.8f, 0.8f),
          Ka(0.2f, 0.2f, 0.2f),
          Ks(1.0f, 1.0f, 1.0f),
          Ke(0.0f, 0.0f, 0.0f)
    {}
};

#endif // MATERIAL_PROPERTIES_H
