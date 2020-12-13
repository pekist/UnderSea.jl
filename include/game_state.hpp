#pragma once
#include "glad/glad.h"
#include "shader_program.hpp"
#include "vertex_buffer.hpp"

class game_state {
public:
  game_state();
  ~game_state();
  void render();

private:
  shader_program _program;
  vertex_buffer<GLfloat[6], GL_ARRAY_BUFFER> _vertices;
};
