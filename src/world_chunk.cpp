#include "world_chunk.hpp"
#include "shader.hpp"
#include <iostream>
#include <stdexcept>

world_chunk::world_chunk(glm::vec2 start, int w, int h)
    : _w(w), _h(h), _start(start), _buffer(1),
      _storage(_buffer.bind<GL_SHADER_STORAGE_BUFFER>(0), GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT | GL_MAP_COHERENT_BIT) {
  shader compute(GL_COMPUTE_SHADER);
  if (!compute.compile("res/generate.glsl")) {
    std::cout << compute.info_log() << std::endl;
    throw std::runtime_error("Failed to compile compute shader");
  }

  _compute_program.attach_shader(compute);
  if (!_compute_program.link_program()) {
    std::cout << _compute_program.info_log() << std::endl;
  }

  glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 3, _buffer[0], 0, sizeof(_storage.get()));
}

void world_chunk::generate() {
  auto p = _compute_program.bind();

  glUniform2fv(0, 1, &_start[0]);
  glDispatchCompute(_w / 16, _h / 16, 1);
  glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);
  auto sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
  glClientWaitSync(sync, 0, 1000000);

  for (int i = 10; i < 10+10; i++) {
    for (int j = 10; j < 10+10; j++) {
        std::cout << _storage.get()[i + j * 512] << " ";
    }
    std::cout << std::endl;
  }
}
