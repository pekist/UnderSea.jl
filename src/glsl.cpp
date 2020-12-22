#include "glsl.hpp"

#include <iostream>

namespace glsl {
shader::shader() {}
shader::variable_info::variable_info(std::string &&name)
    : _name(std::move(name)) {}

std::string shader::compile(const shader_node<void> &m) const {
  std::stringstream out;
  m.compile(out);
  return out.str();
}

} // namespace glsl

// debug code
std::string vertex_shader() {
  glsl::shader vertex;

  const auto in_position = vertex.in<glsl::vec4>("in_position");
  const auto main = vertex.fn<void>("main", [&](){
      return glsl::gl_position.assign(in_position);
  });

  return vertex.compile(main);
};

int main() {
  std::cout << vertex_shader() << "--- END ---" << std::endl;
  return 0;
}
