#ifndef LOG_HPP_6SQ3CRXJ
#define LOG_HPP_6SQ3CRXJ

// #include "Eigen/src/Core/IO.h"
#undef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

#include <spdlog/common.h>
#include <spdlog/fmt/ostr.h>

#include <fmt/format.h>

// #include <Eigen/Core>
// template <typename T>
// struct fmt::formatter<
//     T, typename std::enable_if<
//            std::is_base_of<Eigen::DenseBase<typename std::decay<T>::type>,
//                            typename std::decay<T>::type>::value,
//            char>::type> {
//   Eigen::IOFormat ioformat =
//       Eigen::IOFormat(5, Eigen::DontAlignCols, ", ", ", ", "[", "]", "[", "]");
//
//   template <typename ParseContext>
//   constexpr auto parse(ParseContext &ctx) -> decltype(ctx.begin()) {
//     auto it = ctx.begin(), end = ctx.end();
//     int perc = 0;
//     if (it != end && (*it == '.')) {
//       it++;
//       while (*it >= '0' && *it <= '9') {
//         perc = (perc * 10) + (*it - '0');
//         it++;
//       }
//       ioformat = Eigen::IOFormat(perc, Eigen::DontAlignCols, ", ", ", ", "[",
//                                  "]", "[", "]");
//     }
//
//     // Check if reached the end of the range:
//     if (it != end && *it != '}')
//       throw format_error("invalid format");
//
//     // Return an iterator past the end of the parsed range:
//     return it;
//   }
//
//   template <typename FormatContext>
//   auto format(const T &m, FormatContext &ctx) {
//     return format_to(ctx.out(), "{}", m.format(ioformat));
//   }
// };

#endif /* end of include guard: LOG_HPP_6SQ3CRXJ */
