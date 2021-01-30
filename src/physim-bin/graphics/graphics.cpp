#include "graphics.hpp"
#include "spdlog/common.h"
#include "spdlog/logger.h"
#include "spdlog/spdlog.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "exit_code.hpp"
#include "gui.hpp"
#include "logging.hpp"

namespace graphics {
GLFWwindow *window = nullptr;
} // namespace graphics

std::shared_ptr<spdlog::logger> glfw_logger = nullptr;

void glfw_error_callback(int error, const char *description) {
  glfw_logger->error("GLFW error[{}]: {}", error, description);
}

ExitCode graphics::run() {
  ExitCode exit_code = ExitCode::OK;
  if (window == nullptr) {
    IF_OK(exit_code, initialize("PhySim", 500, 600));
    if (exit_code != ExitCode::OK)
      return exit_code;
  }

  glClearColor(0.0, 0.0, 0.0, 1.0);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    gui::start_frame();

    gui::demo_window();
    gui::plot_demo_window();

    gui::render();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gui::end_frame();
    glfwSwapBuffers(window);
  }

  IF_OK(exit_code, terminate());

  return exit_code;
}
ExitCode graphics::initialize(const std::string &title, unsigned width,
                              unsigned height) {
  glfw_logger = std::make_shared<spdlog::logger>("glfw", logging::sink);
  glfwSetErrorCallback(glfw_error_callback);

  if (!glfwInit()) {
    SPDLOG_CRITICAL("Failed to initialize GLFW");
    return GLFW_INIT_ERROR;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

  window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height),
                            title.c_str(), NULL, NULL);
  if (!window) {
    SPDLOG_CRITICAL("Failed to create GLFW window");
    glfwTerminate();
    return GLFW_CREATE_WINDOW_ERROR;
  }
  SPDLOG_INFO("Created GLFW window {} ({}x{})", title, width, height);

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    SPDLOG_CRITICAL("Failed to initialize OpenGL context");
    return GL_LOAD_ERROR;
  }
  glfwSwapInterval(1);
  SPDLOG_INFO("Initialized OpenGL {}.{} context", 3, 2);
  return gui::initialize(window);
}
ExitCode graphics::terminate() {
  ExitCode exit_code = gui::terminate();
  glfwDestroyWindow(window);
  SPDLOG_INFO("Destroyed GLFW window");
  glfwTerminate();
  SPDLOG_INFO("Terminated other GLFW resources");
  return exit_code;
}
