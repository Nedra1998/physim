#ifndef PHYSIM_LOGGING_HPP_
#define PHYSIM_LOGGING_HPP_

#include "exit_code.hpp"
#include "fmt/chrono.h"
#include "fmt/format.h"
#include "spdlog/common.h"
#include "spdlog/details/log_msg.h"
#include "spdlog/details/null_mutex.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/base_sink.h"

#include <cstdint>
#include <mutex>
#include <string>

namespace logging {
extern spdlog::sink_ptr sink;
extern std::shared_ptr<spdlog::logger> default_logger;
ExitCode initialize(const std::string &color, const std::uint8_t &verbose);

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
    // spdlog::memory_buf_t formatted;
    // spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
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
} // namespace logging

#endif // PHYSIM_LOGGING_HPP_
