#include "object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

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

void Object::Animate(float deltaTime) {
    if (!enableAnimation || controlPoints.size() < 4) return;

    // Incrementar o tempo total acumulado
    timeElapsed += deltaTime;

    // Número total de segmentos (como estamos fechando o ciclo, usamos todos os pontos)
    int totalSegments = controlPoints.size();

    // Tempo total necessário para completar o ciclo
    float totalTimePerSegment = 1.0f;  // Tempo por segmento
    float totalTimeForAllSegments = totalSegments * totalTimePerSegment;

    // Garantir que o tempo seja cíclico
    float currentTime = fmod(timeElapsed, totalTimeForAllSegments);

    // Determinar o segmento atual
    int segment = static_cast<int>(currentTime / totalTimePerSegment);

    // Calcular t dentro do segmento atual (normalizado para [0, 1])
    float t = (currentTime - segment * totalTimePerSegment) / totalTimePerSegment;

    // Ajustar o segmento para fechar o ciclo sem saltos
    segment = segment % totalSegments;

    // Calcular a nova posição
    position = CalculateCatmullRomPosition(segment, t);
}

glm::vec3 Object::CalculateCatmullRomPosition(int segment, float t) {
    if (controlPoints.size() < 4) return position;

    // Garantir continuidade entre o último e o primeiro ponto
    glm::vec3 p0 = controlPoints[(segment - 1 + controlPoints.size()) % controlPoints.size()];
    glm::vec3 p1 = controlPoints[segment % controlPoints.size()];
    glm::vec3 p2 = controlPoints[(segment + 1) % controlPoints.size()];
    glm::vec3 p3 = controlPoints[(segment + 2) % controlPoints.size()];

    // Interpolação Catmull-Rom
    float t2 = t * t;
    float t3 = t2 * t;

    glm::vec3 position = 0.5f * (
        (2.0f * p1) +
        (-p0 + p2) * t +
        (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
        (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3
    );

    return position;
}


