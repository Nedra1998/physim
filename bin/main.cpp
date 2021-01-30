#include <iostream>

#include <cxxopts.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#include <windows.h>
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
#include <unistd.h>
#endif

#include "vis.hpp"

int main(int argc, char *argv[]) {
  cxxopts::Options options("vis");

  options.add_options()("h,help", "print this usage message")(
      "c,color", "enable colored logging",
      cxxopts::value<std::string>()->implicit_value("always")->default_value(
          "auto"));

  auto result = options.parse(argc, argv);
  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    std::exit(0);
  }

  try {
    spdlog::sink_ptr sink;
#if defined(_WIN32) || defined(_WIN64)
    bool isatty = _isatty(_fileno(stdout));
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
    bool isatty = ::isatty(fileno(stdout)) != 0;
#else
    bool isatty = false;
#endif

    if (result["color"].as<std::string>() == "always" ||
        (result["color"].as<std::string>() == "auto" && isatty)) {
      sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    } else {
      sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
    }
    std::shared_ptr<spdlog::logger> default_logger =
        std::make_shared<spdlog::logger>("VIS", sink);
    spdlog::set_default_logger(default_logger);
  } catch (const spdlog::spdlog_ex &ex) {
    std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    return -1;
  }
  test();
  return 0;
}
