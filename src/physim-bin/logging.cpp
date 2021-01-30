#include "logging.hpp"

#include <cstdint>
#include <iostream>
#include <string>

#include <magic_enum.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#include <windows.h>
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
#include <unistd.h>
#endif

#include "exit_code.hpp"

namespace logging {
spdlog::sink_ptr sink = nullptr;
} // namespace logging

ExitCode logging::initialize(const std::string &color,
                             const std::uint8_t &verbose) {
  try {
#if defined(_WIN32) || defined(_WIN64)
    bool isatty = _isatty(_fileno(stdout));
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
    bool isatty = ::isatty(fileno(stdout)) != 0;
#else
    bool isatty = false;
#endif

    if (color == "always" || (color == "auto" && isatty)) {
      sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    } else {
      sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
    }

    std::optional<spdlog::level::level_enum> verbosity =
        magic_enum::enum_cast<spdlog::level::level_enum>(
            magic_enum::enum_integer(spdlog::level::level_enum::off) - verbose);
    if (verbosity.has_value())
      sink->set_level(verbosity.value());

    std::shared_ptr<spdlog::logger> default_logger =
        std::make_shared<spdlog::logger>("physim", sink);
    spdlog::set_default_logger(default_logger);
  } catch (const spdlog::spdlog_ex &ex) {
    std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    return SPDLOG_INIT_ERROR;
  }
  return OK;
}
