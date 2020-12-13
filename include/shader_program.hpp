#pragma once
#include "glad/glad.h"

#include "shader.hpp"
#include <initializer_list>

class shader_program {
public:
  shader_program();
  shader_program(std::initializer_list<shader> shaders);
  shader_program(shader_program &&s);

  void attach_shader(shader &s);
  bool link_program();
  std::string info_log();

  GLuint raw_program() const;
  void use();

  static void clear();
  ~shader_program();

  shader_program &operator=(const shader_program &) = delete;
  shader_program(const shader_program &) = delete;

private:
  GLuint _program;
};
