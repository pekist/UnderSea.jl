#include "game_state.hpp"
#include <iostream>
#include <stdexcept>
#include <cstring>
static GLfloat triangle[] = {0.0, 1.0, -1.0, -1.0, 1.0, -1.0};

game_state::game_state() {
  shader vertex_shader(GL_VERTEX_SHADER);
  shader fragment_shader(GL_FRAGMENT_SHADER);
  if (!vertex_shader.compile("res/vertex.glsl")) {
    std::cerr << vertex_shader.info_log() << std::endl;
    throw std::runtime_error("Failed to compile vertex shader");
  }

  if (!fragment_shader.compile("res/fragment.glsl")) {
    std::cerr << fragment_shader.info_log() << std::endl;
    throw std::runtime_error("Failed to compile fragment shader");
  }

  _program.attach_shader(vertex_shader);
  _program.attach_shader(fragment_shader);
  if (!_program.link_program()) {
    std::cerr << _program.info_log() << std::endl;
    throw std::runtime_error("Failed to link shaders");
  }

  _vertices.bind();
  _vertices.attribute_pointer(0, 2, GL_FLOAT, 0,
                              static_cast<void *>(0));
  _vertices.enable_attribute(0);
}

game_state::~game_state() {}

void game_state::render() {
  _program.use();
  auto &f = _vertices.get();
  std::memcpy(f, triangle, sizeof(triangle));
  _vertices.bind();
  glDrawArrays(GL_TRIANGLES, 0, 3);

  decltype(_vertices)::clear();
  shader_program::clear();
}
