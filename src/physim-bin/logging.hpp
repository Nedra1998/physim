#ifndef PHYSIM_LOGGING_HPP_
#define PHYSIM_LOGGING_HPP_

#include "exit_code.hpp"
#include "spdlog/common.h"

#include <cstdint>
#include <string>

namespace logging {
extern spdlog::sink_ptr sink;
ExitCode initialize(const std::string &color, const std::uint8_t &verbose);
} // namespace logging

#endif // PHYSIM_LOGGING_HPP_
