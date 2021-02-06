#include "sysinfo.hpp"
#include "imgui.h"

#include <chrono>

#if defined(_WIN32) || defined(_WIN64)
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
#include <sys/sysinfo.h>
#include <sys/types.h>
#endif

#include <implot.h>

#include "logging.hpp"

graphics::gui::SysInfoWindow::SysInfoWindow(std::size_t size)
    : Window("\uf7fc System Info"), time_axis(size), vram_usage(size),
      pram_usage(size), cpu_usage(size) {
  FILE *file = fopen("/proc/stat", "r");
  fscanf(file, "cpu %llu, %llu, %llu, %llu", &last_cpu_totals[0],
         &last_cpu_totals[1], &last_cpu_totals[2], &last_cpu_totals[3]);
  fclose(file);
}
graphics::gui::SysInfoWindow::~SysInfoWindow() {}

void graphics::gui::SysInfoWindow::update() {
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  std::chrono::duration<double> diff(now - last_update);
  if (!(diff.count() > update_duration) || !(diff.count() > 0.1)) {
    return;
  }
  approx_fps = diff.count() / static_cast<double>(frame_count);
  last_update = now;

#if defined(_WIN32) || defined(_WIN64)
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
  time_axis.push_back(
      std::chrono::duration<double>(now.time_since_epoch()).count());

  struct sysinfo meminfo;
  sysinfo(&meminfo);
  vram_usage.push_back(
      100.0 *
      static_cast<double>((meminfo.totalram - meminfo.freeram) +
                          (meminfo.totalswap - meminfo.freeswap)) /
      static_cast<double>(meminfo.totalram + meminfo.totalswap));
  pram_usage.push_back(100.0 *
                       static_cast<double>(meminfo.totalram - meminfo.freeram) /
                       static_cast<double>(meminfo.totalram));

  FILE *file = fopen("/proc/stat", "r");
  unsigned long long total_user, total_user_low, total_sys, total_idle;
  fscanf(file, "cpu %llu %llu %llu %llu", &total_user, &total_user_low,
         &total_sys, &total_idle);
  fclose(file);

  if (total_user < last_cpu_totals[0] || total_user_low < last_cpu_totals[1] ||
      total_sys < last_cpu_totals[2] || total_idle < last_cpu_totals[3]) {
    if (cpu_usage.size() != 0)
      cpu_usage.push_back(cpu_usage.back());
    else
      cpu_usage.push_back(0.0);
  } else {
    unsigned long long total = (total_user - last_cpu_totals[0]) +
                               (total_user_low - last_cpu_totals[1]) +
                               (total_sys - last_cpu_totals[2]);
    cpu_usage.push_back(
        100.0 * static_cast<double>(total) /
        static_cast<double>(total + (total_idle - last_cpu_totals[3])));
  }
  last_cpu_totals[0] = total_user;
  last_cpu_totals[1] = total_user_low;
  last_cpu_totals[2] = total_sys;
  last_cpu_totals[3] = total_idle;
#endif

  if (vram_usage.back() > 80.0 && vram_usage.prev() <= 80.0) {
    LERR("sysinfo", "High virtual memory usage: {}%", vram_usage.back());
  }
  if (pram_usage.back() > 80.0 && pram_usage.prev() <= 80.0) {
    LWARN("sysinfo", "High physical memory usage: {}%", pram_usage.back());
  }
  if (cpu_usage.back() > 80.0 && cpu_usage.prev() <= 80.0) {
    LERR("sysinfo", "High CPU usage: {}%", cpu_usage.back());
  }
}

void graphics::gui::SysInfoWindow::draw() {
  static constexpr double history_minimum = 1.0, history_maximum = 300.0;
  static constexpr double min_sample_rate = 0.1, max_sample_rate = 1.0;
  ImGui::SliderScalar("SampleRate", ImGuiDataType_Double, &update_duration,
                      &min_sample_rate, &max_sample_rate, "%.2fs");
  ImGui::SliderScalar("History", ImGuiDataType_Double, &history_duration,
                      &history_minimum, &history_maximum, "%.1fs");

  if (update_duration != 0.0 &&
      (time_axis.capacity() <
           static_cast<std::size_t>(history_maximum / update_duration) ||
       time_axis.capacity() >
           static_cast<std::size_t>(history_maximum / update_duration))) {
    std::size_t scroll_buffer_size =
        static_cast<std::size_t>(history_maximum / update_duration);
    time_axis.resize(scroll_buffer_size);
    vram_usage.resize(scroll_buffer_size);
    pram_usage.resize(scroll_buffer_size);
    cpu_usage.resize(scroll_buffer_size);
    LINFO("Resizing scroll buffers to: {}", scroll_buffer_size);
  }

  ImPlot::SetNextPlotLimitsX(time_axis.back() - history_duration,
                             time_axis.back(), ImGuiCond_Always);
  ImPlot::SetNextPlotLimitsY(0.0, 100.0, ImGuiCond_Always);
  if (ImPlot::BeginPlot("##SystemInfo", NULL, NULL, ImVec2(-1, -1), 0,
                        ImPlotAxisFlags_Time)) {
    ImPlot::PlotLine("Virtual Memory", &time_axis.data()[0],
                     &vram_usage.data()[0], static_cast<int>(vram_usage.size()),
                     static_cast<int>(vram_usage.offset), sizeof(double));
    ImPlot::PlotLine("Physical Memory", &time_axis.data()[0],
                     &pram_usage.data()[0], static_cast<int>(pram_usage.size()),
                     static_cast<int>(pram_usage.offset), sizeof(double));
    ImPlot::PlotLine("CPU Usage", &time_axis.data()[0], &cpu_usage.data()[0],
                     static_cast<int>(cpu_usage.size()),
                     static_cast<int>(cpu_usage.offset), sizeof(double));
    ImPlot::EndPlot();
  }
}
