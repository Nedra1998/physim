#include <iostream>

#include <cxxopts.hpp>
#include <magic_enum.hpp>

#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#include <windows.h>
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
#include <unistd.h>
#endif

#include "exit_code.hpp"
#include "logging.hpp"
#include "graphics/graphics.hpp"

ExitCode configure_logging(const cxxopts::ParseResult &args) {
#if defined(_WIN32) || defined(_WIN64)
  bool isatty = _isatty(_fileno(stdout));
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
  bool isatty = ::isatty(fileno(stdout)) != 0;
#else
  bool isatty = false;
#endif

  try {
    spdlog::level::level_enum verbosity =
        magic_enum::enum_cast<spdlog::level::level_enum>(
            args["verbosity"].as<std::uint8_t>())
            .value_or(spdlog::level::level_enum::info);

    std::shared_ptr<spdlog::sinks::dist_sink_mt> dist_sink =
        std::make_shared<spdlog::sinks::dist_sink_mt>();

    if (args["color"].as<std::string>() == "always" ||
        (args["color"].as<std::string>() == "auto" && isatty)) {
      auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      sink->set_level(verbosity);
      dist_sink->add_sink(sink);
    } else {
      auto sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
      sink->set_level(verbosity);
      dist_sink->add_sink(sink);
    }

    {
      auto sink = std::make_shared<physim::logging::buffer_sink_mt>(10);
      dist_sink->add_sink(sink);

      physim::logging::_buffer_sink = sink;
    }

    physim::logging::dist_sink = dist_sink;
  } catch (const spdlog::spdlog_ex &ex) {
    std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    return SPDLOG_INIT_ERROR;
  }
  return OK;
}

int main(int argc, char *argv[]) {
  cxxopts::Options options("physim");

  // clang-format off
  options.add_options()
    ("h,help", "print this usage message")
#ifndef NDEBUG
    ("g,gui", "enable the graphical user interface",
      cxxopts::value<bool>()->implicit_value("true")->default_value("true"));
#else
    ("g,gui", "enable the graphical user interface",
      cxxopts::value<bool>()->implicit_value("true")->default_value("false"));
#endif

  options.add_options("logging")
    ("c,color", "enable colored logging",
      cxxopts::value<std::string>()->implicit_value("always")->default_value("auto"))
    ("v,verbosity", "set verbosity of logging",
      cxxopts::value<std::uint8_t>()->implicit_value("4")->default_value("2"));
  // clang-format on

  auto result = options.parse(argc, argv);
  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    return OK;
  }

  ExitCode exit_code = OK;
  IF_OK(exit_code, configure_logging(result));

  if (result["gui"].as<bool>() == true) {
    IF_OK(exit_code, graphics::run());
  }

  return OK;
}
