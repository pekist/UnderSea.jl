#include "game_state.hpp"
#include <cstring>
#include <iostream>
#include <stdexcept>
static GLfloat triangle[] = {0.0, 1.0, -1.0, -1.0, 1.0, -1.0};

game_state::game_state() : _buffers(1), _attributes(1), _vertices(_buffers.bind<GL_ARRAY_BUFFER>(0)) {
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

  auto attrib = _attributes.bind(0);

  attrib.pointer(0, 2, GL_FLOAT, 0, nullptr);
  attrib.enable(0);
}

game_state::~game_state() {}

void game_state::render() {
  auto attrib = _attributes.bind(0);
  auto binding = _buffers.bind<GL_ARRAY_BUFFER>(0);
  _program.use();
  auto &f = _vertices.get();
  std::memcpy(f, triangle, sizeof(triangle));

  glDrawArrays(GL_TRIANGLES, 0, 3);

  shader_program::clear();
}
