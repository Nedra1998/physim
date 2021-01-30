#include "gui.hpp"

#include <cstdio>
#include <map>
#include <memory>
#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <spdlog/spdlog.h>

#include "exit_code.hpp"
#include "graphics/window.hpp"

#include "iosevka_bold.h"
#include "iosevka_bold_italic.h"
#include "iosevka_italic.h"
#include "iosevka_regular.h"

namespace graphics::gui {
std::map<std::string, std::vector<std::shared_ptr<graphics::gui::Window>>>
    window_map;
std::map<std::string, std::map<std::string, ImFont *>> font_map;
} // namespace graphics::gui

ExitCode graphics::gui::initialize(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  float size_pixels = 18;
  static const ImWchar glyph_ranges[] = {0x0020, 0x00ff, 0xe5fa, 0xe62b, 0xe700,
                                         0xe7c5, 0xf000, 0xf2e0, 0xe200, 0xe2a9,
                                         0xf500, 0xfd46, 0xe300, 0xe3eb, 0xf400,
                                         0xf4a8, 0xf300, 0xf313, 0};

  ImFontConfig regular_cfg;
  snprintf(regular_cfg.Name, sizeof(regular_cfg.Name), "Iosevka Regular");
  font_map["iosevka"]["regular"] =
      io.Fonts->AddFontFromMemoryCompressedBase85TTF(
          iosevka_regular_compressed_data_base85, size_pixels, &regular_cfg,
          glyph_ranges);

  ImFontConfig italic_cfg;
  snprintf(italic_cfg.Name, sizeof(italic_cfg.Name), "Iosevka Italic");
  font_map["iosevka"]["italic"] =
      io.Fonts->AddFontFromMemoryCompressedBase85TTF(
          iosevka_italic_compressed_data_base85, size_pixels, &italic_cfg,
          glyph_ranges);

  ImFontConfig bold_cfg;
  snprintf(bold_cfg.Name, sizeof(bold_cfg.Name), "Iosevka Bold");
  font_map["iosevka"]["bold"] = io.Fonts->AddFontFromMemoryCompressedBase85TTF(
      iosevka_bold_compressed_data_base85, size_pixels, &bold_cfg,
      glyph_ranges);

  ImFontConfig bold_italic_cfg;
  snprintf(bold_italic_cfg.Name, sizeof(bold_italic_cfg.Name),
           "Iosevka Bold Italic");
  font_map["iosevka"]["bolditalic"] =
      io.Fonts->AddFontFromMemoryCompressedBase85TTF(
          iosevka_bold_italic_compressed_data_base85, size_pixels,
          &bold_italic_cfg, glyph_ranges);

  io.Fonts->Build();

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 150");
  SPDLOG_INFO("Created DearImGUI context");

#ifndef NDEBUG
  window_map["\uf5e3 Debug"].push_back(std::make_shared<ImGuiDemoWindow>());
  window_map["\uf5e3 Debug"].push_back(std::make_shared<ImPlotDemoWindow>());
#endif

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

  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow *backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

void graphics::gui::render_menu_bar() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("\ufaae Windows")) {
      for (auto &it : window_map) {
        if (ImGui::BeginMenu(it.first.c_str())) {
          for (auto &win : it.second) {
            ImGui::MenuItem(win->window_title.c_str(), NULL, &win->show_window);
          }
          ImGui::EndMenu();
        }
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
  for (auto &it : window_map) {
    for (auto &win : it.second) {
      win->render();
    }
  }
}

// static bool show_demo_window = true;
// void graphics::gui::demo_window() {
//   if (show_demo_window)
//     ImGui::ShowDemoWindow(&show_demo_window);
// }
//
// static bool show_plot_demo_window = true;
// void graphics::gui::plot_demo_window() {
//   if (show_plot_demo_window)
//     ImPlot::ShowDemoWindow(&show_plot_demo_window);
// }
