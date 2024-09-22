#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "shader.h"
#include "mesh.h"

class Model {
public:
    Model(const std::string &path);
    void Draw(Shader &shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(const std::string &path);
};

#endif
