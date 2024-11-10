#include "object.h"
#include <glm/gtc/matrix_transform.hpp>

Object::Object(const std::string& path, glm::vec3 startPosition) 
    : model(path), position(startPosition), rotationX(0.0f), rotationY(0.0f), isSelected(false) {}

void Object::Draw() {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));

    model.shader.setMat4("model", modelMatrix);
    model.Draw();
}

void Object::Rotate(float deltaX, float deltaY) {
    rotationX += deltaX;
    rotationY += deltaY;
}

void Object::Translate(float deltaX, float deltaY, float deltaZ) {
    position += glm::vec3(deltaX, deltaY, deltaZ);
}
