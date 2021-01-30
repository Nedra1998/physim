#ifndef PHYSIM_WINDOW_HPP_
#define PHYSIM_WINDOW_HPP_

#include <string>

#include <spdlog/spdlog.h>
#include <imgui.h>

namespace graphics::gui {
class Window {
public:
  Window(const std::string &title, const ImGuiWindowFlags &flags = 0,
         const bool &show = false)
      : show_window(show), window_title(title), window_flags(flags) {}
  virtual ~Window() {}

  inline virtual void render() {
    if (show_window) {
      SPDLOG_TRACE("Rendering ImGui window \"{}\"", window_title);
      if (ImGui::Begin(window_title.c_str(), &show_window, window_flags)) {
        this->draw();
      }
      ImGui::End();
    }
  }
  virtual void draw(){};

  bool show_window = true;
  std::string window_title = "";
  ImGuiWindowFlags window_flags = 0;
};

class ImGuiDemoWindow : public Window {
public:
  ImGuiDemoWindow() : Window("\uf8d6 Dear ImGui Demo") {}
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

#endif // PHYSIM_WINDOW_HPP_
