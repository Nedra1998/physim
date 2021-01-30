#include "window.hpp"

#include <imgui.h>
#include <implot.h>

#include "gui.hpp"

void graphics::gui::ImGuiDemoWindow::render() {
  if (show_window) {
    SPDLOG_TRACE("Rendering ImGui window \"{}\"", window_title);
    ImGui::ShowDemoWindow(&show_window);
  }
}
void graphics::gui::ImPlotDemoWindow::render() {
  if (show_window) {
    SPDLOG_TRACE("Rendering ImGui window \"{}\"", window_title);
    ImPlot::ShowDemoWindow(&show_window);
  }
}
