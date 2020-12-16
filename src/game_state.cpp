#include "game_state.hpp"
#include <cstring>
#include <glm/ext.hpp>
#include <iostream>
#include <stdexcept>

static GLfloat mesh[] = {
    -1.0, -1.0, 0.0, 1.0, 0.0, 0.0,
    -1.0, 1.0,  0.0, 1.0, 0.0, 1.0,
    1.0,  1.0,  0.0, 1.0, 1.0, 1.0,
    1.0,  1.0,  0.0, 1.0, 1.0, 1.0,
    1.0,  -1.0, 0.0, 1.0, 1.0, 0.0,
    -1.0, -1.0, 0.0, 1.0, 0.0, 0.0};


game_state::game_state(window &w)
    : _window(w), _buffers(2), _attributes(1),
      _vertices(_buffers.bind<decltype(_vertices)::Buffer_Type>(0)),
      _matrices(_buffers.bind<decltype(_matrices)::Buffer_Type>(1)),
      _chunk(glm::vec2(1.0f, 1.0f), 0.05f),
      _texture(0, 256, 256, &_chunk.chunk[0][0]) {
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
  attrib.pointer(0, 4, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
  attrib.pointer(1, 2, GL_FLOAT, 6 * sizeof(GLfloat), &static_cast<float *>(nullptr)[4]);

  attrib.enable(0);
  attrib.enable(1);
  glBindBufferRange(GL_UNIFORM_BUFFER, 2, _buffers[1], 0,
                    sizeof(matrices_uniform));

  glm::mat4 half(1.0f);
  half = glm::translate(half, glm::vec3(0.0, 0.0, -3.0f));

  _window.upload_perspective(_matrices.get().projection);
  std::memcpy(_matrices.get().view, &half[0][0], sizeof(half));
}

game_state::~game_state() {}

void game_state::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _window.upload_perspective(_matrices.get().projection);

  std::memcpy(_vertices.get(), mesh, sizeof(mesh));

  auto uniforms = _buffers.bind<decltype(_matrices)::Buffer_Type>(1);
  auto attrib = _attributes.bind(0);
  auto program = _program.bind();

  glDrawArrays(GL_TRIANGLES, 0, 6);
}
