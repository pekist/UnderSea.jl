#pragma once
#include "glad/glad.h"

#include <string>

class shader {
public:
  shader(GLenum shader_type);
  shader(shader &&s);

  bool compile(const std::string &file);
  std::string info_log();

  GLuint raw_shader() const;

  ~shader();

  shader &operator=(const shader &) = delete;
  shader(const shader &) = delete;

private:
  GLuint _shader;
};
