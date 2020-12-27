#include "attributes.hpp"

attribute_binding::attribute_binding(GLuint n) { glBindVertexArray(n); }
attribute_binding::attribute_binding(attribute_binding &&) {}
attribute_binding::~attribute_binding() { glBindVertexArray(0); }

void attribute_binding::pointer(GLuint i, GLint size, GLenum type,
                                GLsizei stride, const void *offset) {
    glVertexAttribPointer(i, size, type, false, stride, offset);
}

void attribute_binding::enable(GLuint i) { glEnableVertexAttribArray(i); }

attributes::attributes(int n) : _attributes(n) {
  glGenVertexArrays(n, _attributes.data());
}

attributes::attributes(attributes &&attribs)
    : _attributes(std::move(attribs._attributes)) {}

attributes::~attributes() {
  glDeleteVertexArrays(_attributes.size(), _attributes.data());
}

attribute_binding attributes::bind(int n) {
  return attribute_binding(_attributes[n]);
}

GLuint attributes::operator[](int n) { return _attributes[n]; };
