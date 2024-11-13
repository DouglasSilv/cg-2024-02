#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include "model.h"
#include "shader.h"

class Object {
public:
    Model model;
    glm::vec3 position, scale;
    float rotationX, rotationY, rotationZ;
    bool isSelected;
    bool enableAnimation = false;
    std::vector<glm::vec3> controlPoints;

    Object(const std::string& path, glm::vec3 startPosition);
    void Draw();
    void Rotate(float deltaX, float deltaY, float deltaZ);
    void Translate(float deltaX, float deltaY, float deltaZ);
    void SetRotation(const glm::vec3& rotation);
    void SetScale(const glm::vec3& scale);
    void Animate(float time);

private:
    glm::vec3 CalculateHermitePosition(float t);
};

#endif
