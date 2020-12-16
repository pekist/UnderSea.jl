#include "world_chunk.hpp"

#include <functional>
#include <glm/gtc/noise.hpp>
#include <iostream>

world_chunk::world_chunk(glm::vec2 start, float inc) {
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < 256; j++) {
        chunk[i][j] = std::hash<float>{}((start.x + inc * i) * (start.y + inc * j));
    }
  }
}
