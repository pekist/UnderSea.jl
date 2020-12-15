#pragma once
#include <glad/glad.h>

#include "attributes.hpp"
#include "buffer_storage.hpp"
#include "buffers.hpp"

#include "shader_program.hpp"
#include "window.hpp"

struct matrices_uniform {
  GLfloat projection[16];
  GLfloat view[16];
};

class game_state {
public:
  game_state(window &);
  ~game_state();
  void render();

private:
  window &_window;
  shader_program _program;
  attributes _attributes;
  buffers _buffers;
  buffer_storage<GLfloat[6], GL_ARRAY_BUFFER> _vertices;
  buffer_storage<matrices_uniform, GL_UNIFORM_BUFFER> _matrices;
};
