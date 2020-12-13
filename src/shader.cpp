#include <glad/glad.h>

#include "shader.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

shader::shader(GLenum shader_type) : _shader(glCreateShader(shader_type)) {}

shader::shader(shader &&s) : _shader(std::exchange(s._shader, 0)) {}

bool shader::compile(const std::string &file) {
  std::ifstream fs;
  std::ostringstream sstr;

  fs.open(file, std::ios::in);
  sstr << fs.rdbuf();
  auto src = sstr.str();
  fs.close();

  int len[] = {static_cast<int>(src.length())};
  const char *sources[] = {src.c_str()};
  glShaderSource(_shader, 1, sources, len);
  glCompileShader(_shader);
  int status;
  glGetShaderiv(_shader, GL_COMPILE_STATUS, &status);
  return status != 0;
}

std::string shader::info_log() {
  int len;
  glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &len);
  if (len == 0) {
    return std::string();
  }
  std::vector<char> info_log(len);
  glGetShaderInfoLog(_shader, len, nullptr, info_log.data());
  return std::string(info_log.data(), len);
}

shader::~shader() {
  if (_shader != 0) {
    glDeleteShader(_shader);
  }
}

GLuint shader::raw_shader() const { return _shader; }
