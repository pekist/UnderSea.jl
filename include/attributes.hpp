#include "glad/glad.h"
#include <vector>

class attribute_binding {
public:
  attribute_binding(GLuint n);
  attribute_binding(attribute_binding &&);

  ~attribute_binding();

  void enable(GLuint i);
  void pointer(GLuint i, GLint size, GLenum type, GLsizei stride,
                         const void *offset);
  attribute_binding(const attribute_binding &) = delete;
  attribute_binding &operator=(const attribute_binding &) = delete;
};

class attributes {
public:
  attributes(int n);
  attributes(attributes &&);

  GLuint operator[](int n);
  attribute_binding bind(int n);

  ~attributes();

  attributes(const attribute_binding &) = delete;
  attributes &operator=(const attribute_binding &) = delete;

private:
  std::vector<GLuint> _attributes;
};
