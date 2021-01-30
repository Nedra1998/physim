#include "gui.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <spdlog/spdlog.h>

#include "exit_code.hpp"

namespace graphics::gui {} // namespace graphics::gui

ExitCode graphics::gui::initialize(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 150");
  SPDLOG_INFO("Created DearImGUI context");
  return ExitCode::OK;
}
ExitCode graphics::gui::terminate() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImPlot::DestroyContext();
  ImGui::DestroyContext();
  SPDLOG_INFO("Destroyed DearImGUI context");
  return ExitCode::OK;
}

void graphics::gui::render() { ImGui::Render(); }
void graphics::gui::start_frame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}
void graphics::gui::end_frame() {
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  // Update and Render additional Platform Windows
  // (Platform functions may change the current OpenGL context, so we
  // save/restore it to make it easier to paste this code elsewhere.
  //  For this specific demo app we could also call
  //  glfwMakeContextCurrent(window) directly)
  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow *backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

static bool show_demo_window = true;
void graphics::gui::demo_window() {
  if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);
}

static bool show_plot_demo_window = true;
void graphics::gui::plot_demo_window() {
  if (show_plot_demo_window)
    ImPlot::ShowDemoWindow(&show_plot_demo_window);
}
