#include "game_state.hpp"
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>

int main(int argc, char **argv) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Learn", NULL, NULL);

  if (window == nullptr) {
    std::cout << "Failed to create opengl window" << std::endl;
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return 1;
  }

  glViewport(0, 0, 800, 600);
  glClearColor(0,0,0,1);
  game_state game;

  while(!glfwWindowShouldClose(window)){
      glClear(GL_COLOR_BUFFER_BIT);

      game.render();

      glfwSwapBuffers(window);
      glfwPollEvents();
  }

  return 0;
}
