#ifndef PHYSIM_WINDOW_HPP_
#define PHYSIM_WINDOW_HPP_

#include "logging.hpp"
#include "spdlog/common.h"
#include <algorithm>
#include <chrono>
#include <string>

#include <imgui.h>
#include <spdlog/spdlog.h>

namespace graphics::gui {

namespace detail {
template <typename ToDuration>
inline ToDuration time_fraction(spdlog::log_clock::time_point tp) {
  auto duration = tp.time_since_epoch();
  auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration);
  return std::chrono::duration_cast<ToDuration>(duration) -
         std::chrono::duration_cast<ToDuration>(secs);
}
} // namespace detail

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
class SpdlogWindow : public Window {
public:
  enum ColumnID {
    TIMESTAMP,
    LEVEL,
    MSG,
    FILENAME,
    FUNCNAME,
    LINE,
    THREAD,
    LOGGER
  };
  SpdlogWindow(std::size_t buffer = 100);
  virtual ~SpdlogWindow();
  inline void draw() override;

private:
  // static bool compare_with_sort(const logging::LogMsg &a,
  //                               const logging::LogMsg &b);
  // static const ImGuiTableSortSpecs *current_sort_specs;

  std::size_t buffer_size;
  std::shared_ptr<logging::buffer_sink_mt> sink;
  std::vector<logging::LogMsg> buffer;
};
} // namespace graphics::gui

#endif // PHYSIM_WINDOW_HPP_
