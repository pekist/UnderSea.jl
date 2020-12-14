#pragma once
#include "glad/glad.h"

#include <vector>

template <const GLenum Type> class binding {
public:
  binding(GLuint buf) {
    glBindBuffer(Type, buf);
  }
  binding(binding &&){};
  ~binding() {
    glBindBuffer(Type, 0);
  }

  binding(const binding &) = delete;
  binding &operator=(const binding &) = delete;
};

class buffers {
public:
  buffers(int n);
  buffers(buffers &&);

  GLuint operator[](int n);

  template <const GLenum Type> binding<Type> bind(int n) {
    return binding<Type>(_buffers[n]);
  }

  ~buffers();

  buffers(const buffers &) = delete;
  buffers &operator=(const buffers &) = delete;

private:
  std::vector<GLuint> _buffers;
};
