#ifndef WINDOW_DEMO_HPP_
#define WINDOW_DEMO_HPP_

#include <string>

#include <imgui.h>

#include "base.hpp"

namespace graphics::gui {
class ImGuiDemoWindow : public Window {
public:
  ImGuiDemoWindow() : Window("\uf8d6 Dear ImGui Demo", 0, true) {}
  virtual ~ImGuiDemoWindow() {}
  void render() override;
  inline void draw() override {}
};
class ImPlotDemoWindow : public Window {
public:
  ImPlotDemoWindow() : Window("\uf8d6 ImPlot Demo") {}
  virtual ~ImPlotDemoWindow() {}
  void render() override;
  inline void draw() override {}
};
} // namespace graphics::gui

#endif /* end of include guard: WINDOW_DEMO_HPP_ */
