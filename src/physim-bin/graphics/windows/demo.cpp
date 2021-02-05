#include "demo.hpp"

#include <imgui.h>
#include <implot.h>

void graphics::gui::ImGuiDemoWindow::render() {
  if (show_window) {
    ImGui::ShowDemoWindow(&show_window);
  }
}
void graphics::gui::ImPlotDemoWindow::render() {
  if (show_window) {
    ImPlot::ShowDemoWindow(&show_window);
  }
}

