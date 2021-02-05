#ifndef WINDOW_SPDLOG_HPP_
#define WINDOW_SPDLOG_HPP_

#include <chrono>
#include <regex>
#include <string>

#include "base.hpp"

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
  bool sort(const physim::logging::LogMsg &lhs,
            const physim::logging::LogMsg &rhs) const;
  const ImGuiTableSortSpecs *current_sort_specs = nullptr;

  std::size_t buffer_size;
  std::shared_ptr<physim::logging::buffer_sink_mt> sink;
  std::vector<physim::logging::LogMsg> buffer;
  std::string regex_filter_str = "";
  std::string compiled_regex_str = "";
  std::optional<std::regex> regex_filter;
  int level_filter = spdlog::level::level_enum::trace;
};
} // namespace graphics::gui

#endif /* end of include guard: WINDOW_SPDLOG_HPP_ */
