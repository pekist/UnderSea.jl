#pragma once

#include "glad/glad.h"
#include <cstddef>
#include <utility>
#include <vector>

template <typename T, const GLenum Type> class vertex_buffer {
public:
  vertex_buffer() {
    glGenBuffers(1, &_object);
    glBindBuffer(Type, _object);
    auto flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
    glBufferStorage(Type, sizeof(T), nullptr, flags);
    _buffer = static_cast<T *>(glMapBufferRange(Type, 0, sizeof(T), flags));

    glGenVertexArrays(1, &_attributes);
  }

  ~vertex_buffer() {
    if (_buffer != nullptr) {
      glDeleteBuffers(1, &_object);
      _buffer = nullptr;
    }
  }

  void bind() {
    glBindVertexArray(_attributes);
    glBindBuffer(Type, _object);
  }

  static void clear() {
    glBindBuffer(Type, 0);
    glBindVertexArray(0);
  }

  void attribute_pointer(GLuint index, GLint size, GLenum type, GLsizei stride,
                         const void *pointer) {
    glVertexAttribPointer(index, size, type, false, stride, pointer);
  }

  void enable_attribute(GLuint index) { glEnableVertexAttribArray(index); }

  vertex_buffer &operator=(const vertex_buffer &) = delete;
  vertex_buffer(const vertex_buffer &) = delete;

  vertex_buffer(vertex_buffer &&vb)
      : _object(std::exchange(vb._object, 0)),
        _buffer(std::exchange(vb._buffer, nullptr)) {}

  T &get() { return *_buffer; }

private:
  GLuint _object;
  GLuint _attributes;
  T *_buffer;
};
