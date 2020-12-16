#include "world_chunk.hpp"

#include <functional>
#include <glm/gtc/noise.hpp>
#include <iostream>

world_chunk::world_chunk(glm::vec2 start, float inc) {
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < 256; j++) {
      chunk[i][j] = GLubyte(
          256 * glm::mix(glm::simplex(
                             glm::vec2(start.x + i * inc, start.y + j * inc)),
                         glm::perlin(glm::vec2((start.x + i * inc) / 2,
                                                (start.y + j * inc) / 2)),
                         0.3));
    }
  }
}
