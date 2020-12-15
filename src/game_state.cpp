#include "game_state.hpp"
#include <cstring>
#include <glm/ext.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include <stdexcept>

static GLfloat triangle[] = {0.0, 0.5, -0.5, -0.5, 0.5, -0.5};

game_state::game_state()
    : _buffers(2), _attributes(1),
      _vertices(_buffers.bind<decltype(_vertices)::Buffer_Type>(0)),
      _matrices(_buffers.bind<decltype(_matrices)::Buffer_Type>(1)) {
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

  auto binding1 = _buffers.bind<decltype(_vertices)::Buffer_Type>(0);
  auto binding2 = _buffers.bind<decltype(_matrices)::Buffer_Type>(1);
  auto attrib = _attributes.bind(0);

  attrib.pointer(0, 2, GL_FLOAT, 0, nullptr);
  attrib.enable(0);

  glBindBufferRange(GL_UNIFORM_BUFFER, 2, _buffers[1], 0,
                    sizeof(matrices_uniform));

  glm::mat4 persp =
      glm::perspective(glm::radians(45.0f), 400.0f / 300.0f, 0.1f, 100.0f);
  glm::mat4 half(1.0f);
  half = glm::scale(half, glm::vec3(0.4f));

  std::memcpy(&_matrices.get().projection, &persp[0][0], sizeof(persp));
  std::memcpy(&_matrices.get().view, &half[0][0], sizeof(half));
}

game_state::~game_state() {}

void game_state::render() {
  std::memcpy(_vertices.get(), triangle, sizeof(triangle));
  auto uniforms = _buffers.bind<decltype(_matrices)::Buffer_Type>(1);
  auto attrib = _attributes.bind(0);
  auto program = _program.bind();
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
