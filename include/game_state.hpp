#pragma once
#include "attributes.hpp"
#include "buffer_storage.hpp"
#include "buffers.hpp"
#include "glad/glad.h"
#include "shader_program.hpp"

class game_state {
public:
  game_state();
  ~game_state();
  void render();

private:
  shader_program _program;
  attributes _attributes;
  buffers _buffers;
  buffer_storage<GLfloat[6], GL_ARRAY_BUFFER> _vertices;
};
