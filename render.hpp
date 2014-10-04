#pragma once  

#include <vector>
#include <map>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


typedef struct RenderComponent{
    glm::vec3 vertex[2];
} RenderComponent;
