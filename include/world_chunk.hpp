#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class world_chunk {
public:
  world_chunk(glm::vec2 start, float inc);
  GLubyte chunk[256][256];
};
