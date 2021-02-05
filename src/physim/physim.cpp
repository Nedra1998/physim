#include "physim.hpp"
// #include <Eigen/Dense>

#include "logging.hpp"
#include "linalg.hpp"

void test() {
  physim::linalg::dMatrix<4, 4> a(5, -4, -2, -1, 6, 2, -3, 0, 1, 3, 4, 7, 8, 9, 10, 11);
  physim::linalg::dMatrix<4, 3> b(5, 12, -2, 6, 13, 14, 15, 1, 2, 4, 3, -3);
  fmt::print("{} * {} = {}\n", a, b, a * b);
  // vml::dMatrix2x3 m{1, 2, 3, 4, 5, 6};
  // vml::dVector3 v(10, 11, 12);
  // fmt::print("{} -> {}\n", m, m.transpose());
  // fmt::print("{} -> {}\n", v, v.transpose());
  // SPDLOG_INFO("This is a matrix: {}", m);
  // SPDLOG_INFO("This is a matrix: {:.10}", m);
}
