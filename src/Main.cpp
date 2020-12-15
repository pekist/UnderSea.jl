#include "game_state.hpp"
#include "window.hpp"

#include <iostream>

int main(int argc, char **argv) {

  glfw glfw_context;
  window win = glfw_context.create_window(800, 600);
  game_state game(win);

  while (!win.should_close()) {
    game.render();

    win.swap_buffers();
    win.poll_events();
  }

  return 0;
}
