#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "model.h"
#include "camera.h"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float fov = 45.0f; // Campo de visão inicial da câmera
float rotationAngleY = 0.0f; // Rotação em torno do eixo Y (esquerda/direita)
float rotationAngleX = 0.0f; // Rotação em torno do eixo X (cima/baixo)
float deltaTime = 0.0f; // Tempo entre o frame atual e o frame anterior
float lastFrame = 0.0f; // Tempo do último frame
float screenWidth = 800;
float screenHeight = 600;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f); // Posição da luz

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float rotationSpeed = 50.0f; // Velocidade de rotação

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        rotationAngleY -= rotationSpeed * deltaTime; // Rotaciona para a esquerda
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        rotationAngleY += rotationSpeed * deltaTime; // Rotaciona para a direita
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        rotationAngleX -= rotationSpeed * deltaTime; // Rotaciona para cima
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        rotationAngleX += rotationSpeed * deltaTime; // Rotaciona para baixo
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // Ajusta o campo de visão com base no scroll
    fov -= (float)yoffset; 

    // Limitar o valor do fov para evitar zoom extremo
    if (fov < 1.0f)
        fov = 1.0f;
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

    // Carregamento do modelo 3D
    Model ourModel("D:/cg-2024-02/assets/modelo.obj");

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        shader.setVec3("lightPos", lightPos);
        shader.setVec3("viewPos", camera.Position); // Posição da câmera
        shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f)); // Luz branca
        shader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f)); // Cor do objeto

        glm::mat4 projection = glm::perspective(glm::radians(fov), screenWidth / screenHeight, 0.1f, 100.0f);
        shader.setMat4("projection", projection);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f); // Matriz identidade

        // Mantém o objeto no centro
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 

        // Aplica rotação em torno do eixo X (para cima e para baixo)
        model = glm::rotate(model, glm::radians(rotationAngleX), glm::vec3(1.0f, 0.0f, 0.0f));

        // Aplica rotação em torno do eixo Y (para esquerda e direita)
        model = glm::rotate(model, glm::radians(rotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f));

        // Envia a matriz model para o shader
        shader.setMat4("model", model);

        ourModel.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }

    glfwTerminate();
    return 0;
}
