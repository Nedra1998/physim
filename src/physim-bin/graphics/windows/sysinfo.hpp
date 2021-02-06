#ifndef WINDOW_SYSINFO_HPP_
#define WINDOW_SYSINFO_HPP_

#include <vector>
#include <chrono>

#include "base.hpp"

namespace graphics::gui {
class SysInfoWindow : public Window {
public:
  SysInfoWindow(std::size_t size = 600); virtual ~SysInfoWindow();
  void draw() override;
  void update() override;
private:

  std::size_t frame_count = 0;
  double update_duration = 0.5;
  std::chrono::system_clock::time_point last_update;
  double approx_fps = 60.0;
  double history_duration = 60.0;
  ReservedQueue<double> time_axis;
  ReservedQueue<double> vram_usage, pram_usage, cpu_usage;
  std::array<unsigned long long, 4> last_cpu_totals = {0, 0, 0, 0};
};
} // namespace graphics::gui

#endif /* end of include guard: WINDOW_SYSINFO_HPP_ */
