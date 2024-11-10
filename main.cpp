#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "shader.h"
#include "camera.h"
#include "object.h"
#include "json.hpp"

using json = nlohmann::json;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float fov = 45.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float screenWidth = 1800;
float screenHeight = 1000;
float rotationAngleY = 0.0f;
float rotationAngleX = 0.0f;
std::vector<Object> objects;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void loadSceneFromJSON(const std::string& filePath, glm::vec3& lightPos, glm::vec3& lightColor, Camera& camera, float& fov, float& nearPlane, float& farPlane) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo JSON: " << filePath << std::endl;
        return;
    }

    json sceneJson;
    file >> sceneJson;

    // Carregar posição e cor da luz
    lightPos = glm::vec3(
        sceneJson["light"]["position"][0].get<float>(),
        sceneJson["light"]["position"][1].get<float>(),
        sceneJson["light"]["position"][2].get<float>()
    );
    lightColor = glm::vec3(
        sceneJson["light"]["color"][0].get<float>(),
        sceneJson["light"]["color"][1].get<float>(),
        sceneJson["light"]["color"][2].get<float>()
    );

    // Carregar dados da câmera
    glm::vec3 cameraPosition(
        sceneJson["camera"]["position"][0].get<float>(),
        sceneJson["camera"]["position"][1].get<float>(),
        sceneJson["camera"]["position"][2].get<float>()
    );
    glm::vec3 cameraOrientation(
        sceneJson["camera"]["orientation"][0].get<float>(),
        sceneJson["camera"]["orientation"][1].get<float>(),
        sceneJson["camera"]["orientation"][2].get<float>()
    );
    fov = sceneJson["camera"]["fov"].get<float>();
    nearPlane = sceneJson["camera"]["near"].get<float>();
    farPlane = sceneJson["camera"]["far"].get<float>();

    // Aplicar posição e orientação inicial da câmera
    camera.Position = cameraPosition;
    camera.Front = glm::normalize(cameraOrientation);  // Normalizar a orientação para garantir direção correta
}

void select_object(int selectedIndex) {
    for (int index = 1; index <= objects.size() + 1 ; index++){
        if (index == selectedIndex) {
            objects[index - 1].isSelected = true;
        } else {
            objects[index - 1].isSelected = false;
        }
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float rotationSpeed = 2000.0f * deltaTime;
    float translationSpeed = 2.5f * deltaTime; // Velocidade da translação
    rotationAngleY = 0.0f;
    rotationAngleX = 0.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        rotationAngleY -= rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        rotationAngleY += rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        rotationAngleX -= rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        rotationAngleX += rotationSpeed;

    for (Object& object : objects) {
        if (object.isSelected) {
            object.Rotate(rotationAngleX * deltaTime, rotationAngleY * deltaTime);
        }
    }

    // Translação com as setas do teclado
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        for (Object& object : objects) {
            if (object.isSelected) {
                object.Translate(0.0f, translationSpeed, 0.0f);  // Mover para cima (no eixo Y)
            }
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        for (Object& object : objects) {
            if (object.isSelected) {
                object.Translate(0.0f, -translationSpeed, 0.0f);   // Mover para baixo (no eixo Y
            }
        }
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        for (Object& object : objects) {
            if (object.isSelected) {
                object.Translate(-translationSpeed, 0.0f, 0.0f);  // Mover para a esquerda (no eixo X)
            }
        }
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        for (Object& object : objects) {
            if (object.isSelected) {
                object.Translate(translationSpeed, 0.0f, 0.0f);   // Mover para a direita (no eixo X)
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        select_object(1);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        select_object(2);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        select_object(3);
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        select_object(4);
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        select_object(5);
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        select_object(6);
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        select_object(7);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    float translationSpeed = 2.5f * deltaTime;  // Velocidade da translação
    for (Object& object : objects) {
        if (object.isSelected) {
            object.Translate(0.0f, 0.0f, (float)yoffset * translationSpeed);  // Mover para cima/baixo com o scroll
        }
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Visualizador 3D", NULL, NULL);
    if (window == NULL) {
        std::cout << "Falha ao criar janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Falha ao inicializar GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, screenWidth, screenHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader shader("D:/cg-2024-02/shaders/vertex_shader.glsl", "D:/cg-2024-02/shaders/fragment_shader.glsl");

    // Criação dos objetos
    Object object1("D:/cg-2024-02/Modelos3D/Suzannes/SuzanneHigh.obj", glm::vec3(0.0f, 0.0f, 0.0f));
    Object object2("D:/cg-2024-02/Modelos3D/Planetas/planeta.obj", glm::vec3(2.0f, 0.0f, 0.0f));
    objects.push_back(object1);
    objects.push_back(object2);

    glEnable(GL_DEPTH_TEST);

    if (objects.size() > 0) {
        objects[0].isSelected = true;
    }
    
    glm::vec3 lightPos;
    glm::vec3 lightColor;

    // Carregar dados da cena a partir do JSON
    float nearPlane, farPlane;
    loadSceneFromJSON("D:/cg-2024-02/scene.json", lightPos, lightColor, camera, fov, nearPlane, farPlane);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("cameraPos", camera.Position);
        shader.setVec3("lightColor", lightColor);
        shader.setVec3("objectColor", glm::vec3(1.0f, 0.25f, 0.22f));

        glm::mat4 projection = glm::perspective(glm::radians(fov), screenWidth / screenHeight, nearPlane, farPlane);
        shader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        for (Object& object : objects) {
            object.Draw(shader);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
