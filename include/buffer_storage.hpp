#pragma once

#include "buffers.hpp"
#include "glad/glad.h"

#include <cstddef>
#include <utility>
#include <vector>

template <typename T, const GLenum Type> class buffer_storage {
public:
  static const GLenum Buffer_Type = Type;

  buffer_storage(binding<Type> &&t) {
    auto flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
    glBufferStorage(Type, sizeof(T), nullptr, flags);
    _buffer = static_cast<T *>(glMapBufferRange(Type, 0, sizeof(T), flags));
  }

  // cleaned up by the binding type
  ~buffer_storage() {}

  buffer_storage &operator=(const buffer_storage &) = delete;
  buffer_storage(const buffer_storage &) = delete;

  buffer_storage(buffer_storage &&vb)
      : _buffer(std::exchange(vb._buffer, nullptr)) {}

  T &get() { return *_buffer; }

private:
  T *_buffer;
};
