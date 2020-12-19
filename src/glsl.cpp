#include "glsl.hpp"

#include <iostream>

namespace glsl {
shader::shader() {}
shader::variable_info::variable_info(std::string &&name)
    : _name(std::move(name)) {}

program::program(const shader_node<void> &node) : _node(node) {}
void program::compile(std::ostream &out) const { _node.compile(out); }

program shader::main(const shader_node<void> &m) const {
    return program(m);
}

} // namespace glsl

// debug code
void vertex_shader() {
  glsl::shader vertex;

  const auto in_position = vertex.in<glsl::vec4>("in_position");
  const auto main = glsl::gl_position.assign(in_position);

  vertex.main(main).compile(std::cout);
};

int main() {
    vertex_shader();
  return 0;
}
