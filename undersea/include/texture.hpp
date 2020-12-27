#pragma once

#include "glad/glad.h"

class texture {
public:
  texture(GLuint texture_unit, int w, int h, float *ptr);
  ~texture();
  void bind_uniform(GLuint uniform);

private:
  GLuint _texture;
  GLuint _unit;
};
