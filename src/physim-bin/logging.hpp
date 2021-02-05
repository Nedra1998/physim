#ifndef PHYSIM_BIN_LOGGING_HPP_
#define PHYSIM_BIN_LOGGING_HPP_

#include <physim/logging.hpp>

namespace physim::logging {
struct LogMsg {
  spdlog::level::level_enum level;
  std::size_t thread_id;
  spdlog::log_clock::time_point time;
  int line;
  std::string logger_name, filename, funcname, payload;
};

template <typename Mutex>
class buffer_sink : public spdlog::sinks::base_sink<Mutex> {
public:
  buffer_sink(const std::size_t &size) : buffer_size(size) {}
  virtual ~buffer_sink() {}

  std::mutex buffer_mutex;
  std::size_t buffer_size;
  std::vector<LogMsg> buffer;

protected:
  void sink_it_(const spdlog::details::log_msg &msg) override {
    if (buffer.size() < buffer_size) {
      const std::lock_guard<std::mutex> lock(buffer_mutex);
      buffer.push_back(
          {msg.level, msg.thread_id, msg.time, msg.source.line,
           msg.logger_name.data() != 0 ? fmt::to_string(msg.logger_name) : "",
           msg.source.filename != 0 ? fmt::to_string(msg.source.filename) : "",
           msg.source.funcname != 0 ? fmt::to_string(msg.source.funcname) : "",
           msg.payload.data() != 0 ? fmt::to_string(msg.payload) : ""});
    }
  }

  void flush_() override {}
};
using buffer_sink_mt = buffer_sink<std::mutex>;
using buffer_sink_st = buffer_sink<spdlog::details::null_mutex>;

extern std::shared_ptr<buffer_sink_mt> _buffer_sink;
} // namespace physim::logging

#endif /* end of include guard: PHYSIM_BIN_LOGGING_HPP_ */
