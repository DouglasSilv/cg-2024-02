#include "object.h"
#include <glm/gtc/matrix_transform.hpp>

Object::Object(const std::string& path, glm::vec3 startPosition) 
    : model(path), position(startPosition), rotationX(0.0f), rotationY(0.0f), rotationZ(0.0f), isSelected(false) {}

void Object::Draw() {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);

    model.shader.setMat4("model", modelMatrix);
    model.Draw();
}

void Object::Rotate(float deltaX, float deltaY, float deltaZ) {
    rotationX += deltaX;
    rotationY += deltaY;
    rotationZ += deltaZ;
}

void Object::Translate(float deltaX, float deltaY, float deltaZ) {
    position += glm::vec3(deltaX, deltaY, deltaZ);
}

void Object::SetRotation(const glm::vec3& rotation) {
    this->rotationX = rotation.x;
    this->rotationY = rotation.y;
    this->rotationZ = rotation.z;
}

void Object::SetScale(const glm::vec3& scale) {
    this->scale = scale;
}

void Object::Animate(float time) {
    if (!enableAnimation || controlPoints.size() < 4) return;

    float t = fmod(time, 1.0f); // Normaliza o tempo para [0, 1]
    position = CalculateHermitePosition(t);
}

glm::vec3 Object::CalculateHermitePosition(float t) {
    glm::vec3 p0 = controlPoints[0];
    glm::vec3 p1 = controlPoints[1];
    glm::vec3 p2 = controlPoints[2];
    glm::vec3 p3 = controlPoints[3];

    float t2 = t * t;
    float t3 = t2 * t;

    glm::vec3 position = (2 * t3 - 3 * t2 + 1) * p0 +
                         (t3 - 2 * t2 + t) * p1 +
                         (-2 * t3 + 3 * t2) * p2 +
                         (t3 - t2) * p3;

    return position;
}
