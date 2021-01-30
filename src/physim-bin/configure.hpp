#ifndef CONFIGURE_HPP_SIP2YNPO
#define CONFIGURE_HPP_SIP2YNPO
#include "exit_code.hpp"

#include <cstdint>
#include <string>

ExitCode initialize_logger(const std::string &color,
                           const std::uint8_t &verbose);

#endif /* end of include guard: CONFIGURE_HPP_SIP2YNPO */
