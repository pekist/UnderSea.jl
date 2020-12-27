#include "texture.hpp"

texture::texture(GLuint unit, int w, int h, float *ptr) : _unit(unit) {
  glGenTextures(1, &_texture);
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, _texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, w, h, 0, GL_RED, GL_FLOAT, ptr);
  glBindImageTexture(0, _texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

  glActiveTexture(GL_TEXTURE0);
}

void texture::bind_uniform(GLuint uniform) { glUniform1i(uniform, _unit); }

texture::~texture() { glDeleteTextures(1, &_texture); }
