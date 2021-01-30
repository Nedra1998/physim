#include <iostream>

#include <cxxopts.hpp>

#include <physim/physim.hpp>

#include "configure.hpp"
#include "exit_code.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

ExitCode gui() {
  GLFWwindow *window;

  if (!glfwInit())
    return GLFW_INIT_ERROR;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return ERR;
  }

  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  glfwSwapInterval(1);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();
  return OK;
}

int main(int argc, char *argv[]) {
  cxxopts::Options options("physim");

  // clang-format off
  options.add_options()
    ("h,help", "print this usage message")
#ifndef NDEBUG
    ("g,gui", "enable the graphical user interface",
      cxxopts::value<bool>()->implicit_value("true")->default_value("true"));
#else
    ("g,gui", "enable the graphical user interface",
      cxxopts::value<bool>()->implicit_value("true")->default_value("false"));
#endif

  options.add_options("logging")
    ("c,color", "enable colored logging",
      cxxopts::value<std::string>()->implicit_value("always")->default_value("auto"))
    ("v,verbose", "set verbosity of logging",
      cxxopts::value<std::uint8_t>()->implicit_value("4")->default_value("2"));
  // clang-format on

  auto result = options.parse(argc, argv);
  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    return OK;
  }

  ExitCode exit_code = OK;
  IF_OK(exit_code, initialize_logger(result["color"].as<std::string>(),
                                     result["verbose"].as<std::uint8_t>()));

  if (result["gui"].as<bool>() == true) {
    IF_OK(exit_code, gui());
  }

  return OK;
}
