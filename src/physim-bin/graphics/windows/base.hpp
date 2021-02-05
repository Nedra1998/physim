#ifndef WINDOW_BASE_HPP_
#define WINDOW_BASE_HPP_

#include <string>

#include <imgui.h>

#include "logging.hpp"

namespace graphics::gui {
class Window {
public:
  Window(const std::string &title, const ImGuiWindowFlags &flags = 0,
         const bool &show = false)
      : show_window(show), window_title(title), window_flags(flags) {}
  virtual ~Window() {}

  inline virtual void render() {
    if (show_window) {
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
} // namespace graphics::gui

#endif /* end of include guard: WINDOW_BASE_HPP_ */
