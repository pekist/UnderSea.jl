#include "shader_program.hpp"

#include <utility>
#include <vector>
shader_program::shader_program() : _program(glCreateProgram()) {}

shader_program::shader_program(std::initializer_list<shader> shaders)
    : _program(glCreateProgram()) {
  for (auto &s : shaders) {
    glAttachShader(_program, s.raw_shader());
  }
}

void shader_program::attach_shader(shader &s) {
  glAttachShader(_program, s.raw_shader());
}

shader_program::shader_program(shader_program &&s)
    : _program(std::exchange(s._program, 0)) {}

bool shader_program::link_program() {
  glLinkProgram(_program);
  int status;
  glGetProgramiv(_program, GL_LINK_STATUS, &status);
  return status != 0;
}

std::string shader_program::info_log() {
  int len;
  glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &len);
  if (len == 0){
    return std::string();
  }

  std::vector<char> info_log(len);
  glGetProgramInfoLog(_program, len, nullptr, info_log.data());
  return std::string(info_log.data(), len);
}

shader_program::~shader_program() {
  if (_program != 0) {
    glDeleteProgram(_program);
  }
}

GLuint shader_program::raw_program() const { return _program; }
