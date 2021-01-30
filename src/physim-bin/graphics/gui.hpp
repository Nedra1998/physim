#ifndef PHYSIM_GUI_HPP_
#define PHYSIM_GUI_HPP_

#include <optional>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "exit_code.hpp"

namespace graphics::gui {
  ExitCode initialize(GLFWwindow* window);
  ExitCode terminate();
  void render();
  void start_frame();
  void end_frame();

  void demo_window();
  void plot_demo_window();


} // namespace graphics::gui

#endif // PHYSIM_GUI_HPP_
