#include <glad/glad.h>

#include "window.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include <stdexcept>
#include <utility>

glfw::glfw() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

glfw::~glfw() { glfwTerminate(); }

window glfw::create_window(int w, int h) {
  return window(w, h, glfwCreateWindow(w, h, "Under Sea", nullptr, nullptr));
}

window::window(int x, int y, GLFWwindow *w)
    : _window(w), _perspective(glm::perspective(
                      glm::radians(45.0f), float(x) / float(y), 0.1f, 100.0f)) {
  glfwMakeContextCurrent(_window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize GLAD");
  }
  glfwSetWindowUserPointer(w, this);

  glViewport(0, 0, x, y);
  glClearColor(0, 0, 0, 1);
  glfwSetFramebufferSizeCallback(_window, [](GLFWwindow *glfwWindow, int x,
                                             int y) {
    window *w = static_cast<window *>(glfwGetWindowUserPointer(glfwWindow));
    w->_dirty = true;
    if (y != 0) {
      w->_perspective =
          glm::perspective(glm::radians(45.0f), float(x) / float(y), 0.1f, 100.0f);
    }

    glViewport(0, 0, x, y);
  });
}

window::window(window &&w) : _window(std::exchange(w._window, nullptr)) {}

window::~window() {
  if (_window != nullptr) {
    glfwDestroyWindow(_window);
  }
}

void window::swap_buffers() { glfwSwapBuffers(_window); }
void window::poll_events() { glfwPollEvents(); }
bool window::should_close() { return glfwWindowShouldClose(_window); }
bool window::is_dirty() const { return _dirty; }
void window::upload_perspective(float *dst) const {
  std::memcpy(dst, &_perspective[0][0], sizeof(float) * 16);
}
