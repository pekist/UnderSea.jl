#pragma once

#include <glad/glad.h>

#include "buffers.hpp"
#include "buffer_storage.hpp"
#include "shader_program.hpp"
#include "texture.hpp"
#include <glm/glm.hpp>

class world_chunk {
public:
  world_chunk(glm::vec2 start, int w, int h);
  void generate();

private:
  int _w, _h;
  glm::vec2 _start;
  shader_program _compute_program;
  buffers _buffer;
  buffer_storage<GLfloat[512 * 512], GL_SHADER_STORAGE_BUFFER> _storage;
};
