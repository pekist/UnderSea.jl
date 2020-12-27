#pragma once
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class window {
public:
  window(int, int, GLFWwindow *);
  window(window &&);
  ~window();

  window(const window &) = delete;
  window &operator=(const window &) = delete;

  void upload_perspective(float *);

  void swap_buffers();
  void poll_events();
  bool should_close();
  bool is_dirty() const;

private:
  GLFWwindow *_window;

  glm::mat4 _perspective;
  bool _dirty;
};

class glfw {
public:
  glfw();

  ~glfw();

  glfw(glfw &&) = delete;
  glfw(const glfw &) = delete;
  glfw &operator=(const glfw &) = delete;

  window create_window(int, int);
};
