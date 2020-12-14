#pragma once
#include "glad/glad.h"

#include "shader.hpp"
#include <initializer_list>

class program_binding {
public:
  program_binding(GLuint n);
  ~program_binding();

  program_binding(const program_binding &) = delete;
  program_binding &operator=(const program_binding &) = delete;
  program_binding(program_binding &&);
};

class shader_program {
public:
  shader_program();
  shader_program(std::initializer_list<shader> shaders);
  shader_program(shader_program &&s);
  ~shader_program();

  void attach_shader(shader &s);
  bool link_program();
  std::string info_log();

  GLuint raw_program() const;
  program_binding bind();

  shader_program &operator=(const shader_program &) = delete;
  shader_program(const shader_program &) = delete;

private:
  GLuint _program;
};
