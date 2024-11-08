#ifndef MATERIAL_PROPERTIES_H
#define MATERIAL_PROPERTIES_H

#include <glm/glm.hpp>
#include <string>

struct MaterialProperties {
    glm::vec3 Kd; // Coeficiente de reflexão difusa (cor difusa)
    glm::vec3 Ka; // Coeficiente de reflexão ambiente (cor ambiente)
    glm::vec3 Ks; // Coeficiente de reflexão especular (cor especular)
    glm::vec3 Ke; // Emissão de luz (iluminação própria do material)
    
    unsigned int diffuseTexture = 0; // ID da textura difusa (carregada previamente)

    MaterialProperties()
        : Kd(0.8f, 0.8f, 0.8f), // Valores padrão para Kd (cinza claro)
          Ka(0.2f, 0.2f, 0.2f), // Valores padrão para Ka (cinza escuro)
          Ks(1.0f, 1.0f, 1.0f), // Valores padrão para Ks (branco)
          Ke(0.0f, 0.0f, 0.0f)  // Valores padrão para Ke (sem emissão)
    {}
};

#endif // MATERIAL_PROPERTIES_H
