#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include "model.h"
#include "shader.h"

class Object {
public:
    Model model;
    glm::vec3 position;
    float rotationX, rotationY;
    bool isSelected;

    Object(const std::string& path, glm::vec3 startPosition);
    void Draw();
    void Rotate(float deltaX, float deltaY);
    void Translate(float deltaX, float deltaY, float deltaZ);
};

#endif
