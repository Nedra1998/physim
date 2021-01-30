#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <initializer_list>
#include <limits>

#include <fmt/format.h>

#include <physim/linalg.hpp>

using Catch::Approx;
using namespace physim::linalg;

template <typename TestType, std::size_t N, std::size_t M>
inline void test_matrix_equality(const Matrix<TestType, N, M> &m,
                                 const std::initializer_list<TestType> &v) {
  REQUIRE(v.size() == N * M);
  auto it = v.begin();
  for (std::size_t i = 0; i < N * M; ++i) {
    REQUIRE(m[i] == Approx(*it++));
  }
}
template <typename TestType, std::size_t N, std::size_t M>
inline void test_matrix_equality(const Matrix<TestType, N, M> &m,
                                 const TestType v[N * M]) {
  for (std::size_t i = 0; i < N * M; ++i) {
    REQUIRE(m[i] == Approx(v[i]));
  }
}

TEMPLATE_TEST_CASE("Matrix constructors", "[unit][linalg]", bool, int,
                   unsigned int, float, double) {
  TestType mat[2 * 3] = {TestType(0), TestType(1), TestType(2),
                         TestType(3), TestType(4), TestType(5)};
  SECTION("Null constructor") {
    Matrix<TestType, 2, 3> m{};
    test_matrix_equality(m, {TestType(), TestType(), TestType(), TestType(),
                             TestType(), TestType()});
  }
  SECTION("Array constructor") {
    Matrix<TestType, 2, 3> m(mat);
    test_matrix_equality(m, mat);
  }
  SECTION("Scalar constructor") {
    Matrix<TestType, 2, 3> m(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5]);
    test_matrix_equality(m, mat);
  }
  SECTION("Special constructors") {
    Matrix<TestType, 2, 3> m = Matrix<TestType, 2, 3>::Identity();
    test_matrix_equality(m, {TestType(1), TestType(), TestType(), TestType(),
                             TestType(1), TestType()});
  }
}
TEMPLATE_TEST_CASE("Matrix access", "[unit][linalg]", int, unsigned int, float,
                   double) {
  Matrix<TestType, 3, 2> m(TestType(0), TestType(1), TestType(2), TestType(3),
                           TestType(4), TestType(5));
  SECTION("operator()") {
    REQUIRE(m(0, 1) == Approx(TestType(1)));
    REQUIRE(m(1, 1) == Approx(TestType(3)));
    REQUIRE(m(2, 0) == Approx(TestType(4)));
  }
  SECTION("operator[]") {
    REQUIRE(m[1] == Approx(TestType(1)));
    REQUIRE(m[4] == Approx(TestType(4)));
    REQUIRE(m[5] == Approx(TestType(5)));
  }
  SECTION("iterator") {
    REQUIRE(m.begin() == std::begin(m._mat));
    REQUIRE(m.end() == std::end(m._mat));
  }
}

TEMPLATE_TEST_CASE("Matrix simple operators", "[unit][linalg]", int,
                   unsigned int, float, double) {
  Matrix<TestType, 3, 2> a(TestType(0), TestType(2), TestType(4), TestType(6),
                           TestType(8), TestType(10));
  Matrix<TestType, 3, 2> b(TestType(0), TestType(2), TestType(4), TestType(8),
                           TestType(16), TestType(32));
  Matrix<TestType, 2, 3> c(TestType(1), TestType(3), TestType(5), TestType(7),
                           TestType(9), TestType(11));
  SECTION("operator+") {
    test_matrix_equality(a + b, {TestType(0), TestType(4), TestType(8),
                                 TestType(14), TestType(24), TestType(42)});
  }
  SECTION("operator-") {
    test_matrix_equality(b - a, {TestType(0), TestType(0), TestType(0),
                                 TestType(2), TestType(8), TestType(22)});
  }
}

TEMPLATE_TEST_CASE("Matrix multiplication", "[unit][linalg]", int, unsigned int,
                   float, double) {
  SECTION("3x3x3") {
    Matrix<TestType, 3, 3> m(TestType(1), TestType(2), TestType(3), TestType(4),
                             TestType(5), TestType(6), TestType(7), TestType(8),
                             TestType(9));

    test_matrix_equality(m * m, {TestType(30), TestType(36), TestType(42),
                                 TestType(66), TestType(81), TestType(96),
                                 TestType(102), TestType(126), TestType(150)});
  }
  SECTION("4x3x5") {
    Matrix<TestType, 4, 3> a(TestType(1), TestType(2), TestType(3), TestType(4),
                             TestType(5), TestType(6), TestType(7), TestType(8),
                             TestType(9), TestType(10), TestType(11),
                             TestType(12));
    Matrix<TestType, 3, 5> b(
        TestType(12), TestType(11), TestType(10), TestType(9), TestType(8),
        TestType(7), TestType(6), TestType(5), TestType(4), TestType(3),
        TestType(2), TestType(1), TestType(12), TestType(11), TestType(10));
    test_matrix_equality(
        a * b, {TestType(32),  TestType(26),  TestType(56),  TestType(50),
                TestType(44),  TestType(95),  TestType(80),  TestType(137),
                TestType(122), TestType(107), TestType(158), TestType(134),
                TestType(218), TestType(194), TestType(170), TestType(221),
                TestType(188), TestType(299), TestType(266), TestType(233)});
  }
  SECTION("5x8x4") {
    Matrix<TestType, 5, 8> a(
        TestType(1), TestType(2), TestType(3), TestType(4), TestType(5),
        TestType(6), TestType(7), TestType(8), TestType(9), TestType(10),
        TestType(11), TestType(12), TestType(13), TestType(14), TestType(15),
        TestType(16), TestType(17), TestType(18), TestType(19), TestType(20),
        TestType(21), TestType(22), TestType(23), TestType(24), TestType(25),
        TestType(26), TestType(27), TestType(28), TestType(29), TestType(30),
        TestType(31), TestType(32), TestType(33), TestType(34), TestType(35),
        TestType(36), TestType(37), TestType(38), TestType(39), TestType(40));
    Matrix<TestType, 8, 4> b(
        TestType(40), TestType(39), TestType(38), TestType(37), TestType(36),
        TestType(35), TestType(34), TestType(33), TestType(32), TestType(31),
        TestType(30), TestType(29), TestType(28), TestType(27), TestType(26),
        TestType(25), TestType(24), TestType(23), TestType(22), TestType(21),
        TestType(20), TestType(19), TestType(18), TestType(17), TestType(16),
        TestType(15), TestType(14), TestType(13), TestType(12), TestType(11),
        TestType(10), TestType(9));
    test_matrix_equality(a * b, {TestType(768),  TestType(732),  TestType(696),
                                 TestType(660),  TestType(2432), TestType(2332),
                                 TestType(2232), TestType(2132), TestType(4096),
                                 TestType(3932), TestType(3768), TestType(3604),
                                 TestType(5760), TestType(5532), TestType(5304),
                                 TestType(5076), TestType(7424), TestType(7132),
                                 TestType(6840), TestType(6548)});
  }
  SECTION("7x2x3") {
    Matrix<TestType, 7, 2> a(TestType(1), TestType(2), TestType(3), TestType(4),
                             TestType(5), TestType(6), TestType(7), TestType(8),
                             TestType(9), TestType(10), TestType(11),
                             TestType(12), TestType(13), TestType(14));
    Matrix<TestType, 2, 3> b(TestType(14), TestType(13), TestType(12),
                             TestType(11), TestType(10), TestType(9));
    test_matrix_equality(a * b, {TestType(36),  TestType(33),  TestType(30),
                                 TestType(86),  TestType(79),  TestType(72),
                                 TestType(136), TestType(125), TestType(114),
                                 TestType(186), TestType(171), TestType(156),
                                 TestType(236), TestType(217), TestType(198),
                                 TestType(286), TestType(263), TestType(240),
                                 TestType(336), TestType(309), TestType(282)});
  }
  SECTION("2x3x9") {}
  SECTION("8x16x9") {}
}
