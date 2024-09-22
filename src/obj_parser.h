#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <glm/glm.hpp>
#include <vector>
#include "mesh.h"

void loadObj(const std::string& path, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

#endif
