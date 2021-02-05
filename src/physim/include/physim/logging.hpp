#ifndef LOG_HPP_6SQ3CRXJ
#define LOG_HPP_6SQ3CRXJ

#undef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#define SPDLOG_FMT_EXTERNAL

#include <spdlog/common.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#if defined(_WIN32) || defined(_WIN64)
#include <spdlog/sinks/msvc_sink.h>
#else
#include <spdlog/sinks/syslog_sink.h>
#endif
#include <spdlog/spdlog.h>

#define PHYSIM_LOG(logger, level, file, line, function, ...)                   \
  physim::logging::get(logger)->log(spdlog::source_loc{file, line, function},  \
                                    level, __VA_ARGS__)

#define PHYSIM_LOG_TRACE(logger, ...)                                          \
  PHYSIM_LOG(logger, spdlog::level::trace, __FILE__, __LINE__,                 \
             SPDLOG_FUNCTION, __VA_ARGS__)
#define PHYSIM_LOG_DEBUG(logger, ...)                                          \
  PHYSIM_LOG(logger, spdlog::level::debug, __FILE__, __LINE__,                 \
             SPDLOG_FUNCTION, __VA_ARGS__)
#define PHYSIM_LOG_INFO(logger, ...)                                           \
  PHYSIM_LOG(logger, spdlog::level::info, __FILE__, __LINE__, SPDLOG_FUNCTION, \
             __VA_ARGS__)
#define PHYSIM_LOG_WARN(logger, ...)                                           \
  PHYSIM_LOG(logger, spdlog::level::warn, __FILE__, __LINE__, SPDLOG_FUNCTION, \
             __VA_ARGS__)
#define PHYSIM_LOG_ERR(logger, ...)                                            \
  PHYSIM_LOG(logger, spdlog::level::err, __FILE__, __LINE__, SPDLOG_FUNCTION,  \
             __VA_ARGS__)
#define PHYSIM_LOG_CRITICAL(logger, ...)                                       \
  PHYSIM_LOG(logger, spdlog::level::critical, __FILE__, __LINE__,              \
             SPDLOG_FUNCTION, __VA_ARGS__)

#define LTRACE(logger, ...) PHYSIM_LOG_TRACE(logger, __VA_ARGS__)
#define LDEBUG(logger, ...) PHYSIM_LOG_DEBUG(logger, __VA_ARGS__)
#define LINFO(logger, ...) PHYSIM_LOG_INFO(logger, __VA_ARGS__)
#define LWARN(logger, ...) PHYSIM_LOG_WARN(logger, __VA_ARGS__)
#define LERR(logger, ...) PHYSIM_LOG_ERR(logger, __VA_ARGS__)
#define LCRITICAL(logger, ...) PHYSIM_LOG_CRITICAL(logger, __VA_ARGS__)

namespace physim::logging {
extern std::shared_ptr<spdlog::sinks::dist_sink_mt> dist_sink;
void configure_logging();

inline std::shared_ptr<spdlog::logger> get(const std::string &logger) {
  std::shared_ptr<spdlog::logger> spdlog_logger = spdlog::get(logger);
  if (!spdlog_logger) {
    if (dist_sink) {
      spdlog_logger = std::make_shared<spdlog::logger>(logger, dist_sink);
    } else {
      spdlog_logger = std::make_shared<spdlog::logger>(logger);
    }
    spdlog::register_logger(spdlog_logger);
    physim::logging::configure_logging();
  }
  return spdlog_logger;
}
} // namespace physim::logging

#endif /* end of include guard: LOG_HPP_6SQ3CRXJ */
