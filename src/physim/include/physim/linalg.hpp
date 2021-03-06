#ifndef LINALG_HPP_
#define LINALG_HPP_

#include <array>
#include <cstdlib>
#include <cstring>

#include <fmt/format.h>
#include <stdexcept>

#include "logging.hpp"

namespace physim::linalg {
namespace detail {
static constexpr std::size_t mat_mult_min_size = 6;
class MatrixBase {};

template <typename T, typename R = std::size_t>
constexpr inline R floor(const T &num) {
  return static_cast<R>(num);
}
template <typename T, typename U, typename R = std::size_t>
constexpr inline R floor(const T &n, const U &d) {
  return floor<float, R>(static_cast<float>(n) / static_cast<float>(d));
}
template <typename T, typename R = std::size_t>
constexpr inline R ceil(const T &num) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfloat-equal"
  return (static_cast<T>(static_cast<R>(num)) == num)
             ? static_cast<R>(num)
             : static_cast<R>(num) + ((num > 0) ? 1 : 0);
#pragma clang diagnostic pop
}
template <typename T, typename U, typename R = std::size_t>
constexpr inline R ceil(const T &n, const U &d) {
  return ceil<float, R>(static_cast<float>(n) / static_cast<float>(d));
}

template <typename T, std::size_t N, std::size_t M, std::size_t P>
typename std::enable_if<N == 0 || M == 0 || P == 0, void>::type
multiply_ptr(const T **, const T **, T **) {
  throw std::underflow_error("zero-length arrays are not permitted in C++");
}
template <typename T, std::size_t N, std::size_t M, std::size_t P>
typename std::enable_if<N != 0 && M != 0 && P != 0, void>::type
multiply_ptr(const T **a, const T **b, T **c) {
  if constexpr (std::max({N, M, P}) < mat_mult_min_size) {
    for (std::size_t i = 0; i < N; ++i) {
      for (std::size_t j = 0; j < P; ++j) {
        for (std::size_t k = 0; k < M; ++k) {
          *c[i * P + j] += *a[i * M + k] * *b[k * P + j];
        }
      }
    }
  } else if constexpr (std::max({N, M, P}) == N) {
    multiply_ptr<T, detail::floor(N, 2), M, P>(a, b, c);
    multiply_ptr<T, detail::ceil(N, 2), M, P>(a + (N / 2) * M, b,
                                              c + (N / 2) * P);
  } else if constexpr (std::max({N, M, P}) == P) {
    const T *b1[M * detail::floor(P, 2)], *b2[M * detail::ceil(P, 2)];
    T *c1[N * detail::floor(P, 2)], *c2[N * detail::ceil(P, 2)];
    for (std::size_t i = 0; i < M * P; ++i)
      ((i % P < P / 2)
           ? b1[(P / 2) * static_cast<std::size_t>(std::floor(i / P)) + (i % P)]
           : b2[(P / 2) * static_cast<std::size_t>(std::floor(i / P)) +
                (i % P) - (P / 2)]) = b[i];
    for (std::size_t i = 0; i < N * P; ++i)
      ((i % P < P / 2)
           ? c1[(P / 2) * static_cast<std::size_t>(std::floor(i / P)) + (i % P)]
           : c2[(P / 2) * static_cast<std::size_t>(std::floor(i / P)) +
                (i % P) - (P / 2)]) = c[i];
    multiply_ptr<T, N, M, detail::floor(P, 2)>(a, b1, c1);
    multiply_ptr<T, N, M, detail::ceil(P, 2)>(a, b2, c2);
  } else if constexpr (std::max({N, M, P}) == M) {
    const T *a1[N * detail::floor(M, 2)], *a2[N * detail::ceil(M, 2)];
    for (std::size_t i = 0; i < N * M; ++i)
      ((i % M < M / 2)
           ? a1[(M / 2) * static_cast<std::size_t>(std::floor(i / M)) + (i % M)]
           : a2[(M / 2) * static_cast<std::size_t>(std::floor(i / M)) +
                (i % M) - (M / 2)]) = a[i];
    multiply_ptr<T, N, detail::floor(M, 2), P>(a1, b, c);
    multiply_ptr<T, N, detail::ceil(M, 2), P>(a2, b + (M / 2) * P, c);
  }
}
template <typename T, std::size_t N, std::size_t M, std::size_t P>
void multiply(const T a[N * M], const T b[M * P], T c[N * P]) {
  if constexpr (std::max({N, M, P}) < mat_mult_min_size) {
    for (std::size_t i = 0; i < N; ++i) {
      for (std::size_t j = 0; j < P; ++j) {
        T sum = T();
        for (std::size_t k = 0; k < M; ++k) {
          sum += a[i * M + k] * b[k * P + j];
        }
        c[i * P + j] = sum;
      }
    }
  } else {
    const T *ap[N * M], *bp[M * P];
    T *cp[N * P];
    for (std::size_t i = 0; i < N * M; ++i)
      ap[i] = a + i;
    for (std::size_t i = 0; i < M * P; ++i)
      bp[i] = b + i;
    for (std::size_t i = 0; i < N * P; ++i)
      cp[i] = c + i;
    multiply_ptr<T, N, M, P>(ap, bp, cp);
  }
}
} // namespace detail

template <typename T, std::size_t N, std::size_t M>
class Matrix : public detail::MatrixBase {
public:
  typedef T value_type;
  typedef Matrix<T, N, M> matrix_type;

  Matrix() {
    for (std::size_t i = 0; i < N * M; ++i)
      _mat[i] = T();
  }
  Matrix(const T &s) {
    for (std::size_t i = 0; i < N * M; i += (M + 1))
      _mat[i] = s;
  }
  Matrix(const Matrix<T, N, M> &other) {
    for (std::size_t i = 0; i < N * M; ++i)
      _mat[i] = other._mat[i];
  }
  Matrix(const T vals[N * M]) {
    for (std::size_t i = 0; i < N * M; ++i)
      _mat[i] = vals[i];
  }
  template <typename Arg0, typename Arg1, typename... Argn>
  Matrix(const Arg0 &arg0, const Arg1 &arg1, const Argn &...argn) {
    for (std::size_t i = 0; i < N * M; ++i)
      _mat[i] = T();
    constructor<0>(arg0, arg1, argn...);
  }

  static Matrix<T, N, M> Identity(const T &s = T(1)) {
    Matrix<T, N, M> ret;
    ret.diagonal(s);
    return ret;
  }

  virtual ~Matrix() {}

  void diagonal(const T &s) {
    for (std::size_t i = 0; i < N * M; i += (M + 1))
      _mat[i] = s;
  }
  void fill(const T &s) {
    for (std::size_t i = 0; i < N * M; ++i)
      _mat[i] = s;
  }

  Matrix<T, M, N> transpose() const noexcept {
    Matrix<T, M, N> trans;
    for (std::size_t i = 0; i < N; ++i)
      for (std::size_t j = 0; j < M; ++j)
        trans._mat[j * N + i] = _mat[i * M + j];
    return trans;
  }

  constexpr static std::size_t rows() noexcept { return N; }
  constexpr static std::size_t cols() noexcept { return M; }
  constexpr static std::size_t size() noexcept { return N * M; }

  const value_type *begin() const noexcept { return std::begin(_mat); }
  value_type *begin() noexcept { return std::begin(_mat); }
  const value_type *end() const noexcept { return std::end(_mat); }
  value_type *end() noexcept { return std::end(_mat); }

  Matrix<T, N, M> &operator=(const Matrix<T, N, M> &other) {
    for (std::size_t i = 0; i < N * M; ++i)
      _mat[i] = other._mat[i];
    return *this;
  };
  Matrix<T, N, M> operator-() {
    Matrix<T, N, M> res;
    for (std::size_t i = 0; i < N * M; ++i)
      res._mat[i] = -_mat[i];
    return res;
  }

  Matrix<T, N, M> operator+(const Matrix<T, N, M> &rhs) {
    Matrix<T, N, M> res;
    for (std::size_t i = 0; i < N * M; ++i) {
      res._mat[i] = _mat[i] + rhs._mat[i];
    }
    return res;
  }
  Matrix<T, N, M> &operator+=(const Matrix<T, N, M> &rhs) {
    for (std::size_t i = 0; i < N * M; ++i)
      _mat[i] += rhs._mat[i];
    return *this;
  }

  Matrix<T, N, M> operator-(const Matrix<T, N, M> &rhs) {
    Matrix<T, N, M> res;
    for (std::size_t i = 0; i < N * M; ++i)
      res._mat[i] = _mat[i] - rhs._mat[i];
    return res;
  }
  Matrix<T, N, M> &operator-=(const Matrix<T, N, M> &rhs) {
    for (std::size_t i = 0; i < N * M; ++i)
      _mat[i] -= rhs._mat[i];
    return *this;
  }
  template <std::size_t P>
  Matrix<T, N, P> operator*(const Matrix<T, M, P> &rhs) {
    Matrix<T, N, P> res;
    detail::multiply<T, N, M, P>(_mat, rhs._mat, res._mat);
    return res;
  }

  virtual T &operator()(std::size_t i, std::size_t j) {
    return _mat[i * M + j];
  }
  virtual const T &operator()(std::size_t i, std::size_t j) const {
    return _mat[i * M + j];
  }

  T &operator[](std::size_t i) { return _mat[i]; }
  const T &operator[](std::size_t i) const { return _mat[i]; }

  T _mat[N * M];

protected:
  template <std::size_t I = 0, typename Arg0>
  std::enable_if_t<I != 0 && (I < (N * M)), void>
  constructor(const Arg0 &arg0) {
    _mat[I] = arg0;
  }
  template <std::size_t I = 0, typename Arg0, typename Arg1, typename... Argn>
  std::enable_if_t<(I < (N * M)), void>
  constructor(const Arg0 &arg0, const Arg1 &arg1, const Argn &...argn) {
    _mat[I] = arg0;
    constructor<I + 1>(arg1, argn...);
  }
};

template <typename T, std::size_t N>
class SquareMatrix : public Matrix<T, N, N> {
public:
  template <typename... Argn>
  SquareMatrix(const Argn &...argn) : Matrix<T, N, N>(argn...) {}
};

template <typename T, std::size_t N> class Vector : public Matrix<T, N, 1> {
public:
  template <typename... Argn>
  Vector(const Argn &...argn) : Matrix<T, N, 1>(argn...) {}

  T &operator()(std::size_t i) { return Matrix<T, N, 1>::_mat[i]; }
  const T &operator()(std::size_t i) const { return Matrix<T, N, 1>::_mat[i]; }
};

template <typename T> using Matrix1x1 = SquareMatrix<T, 1>;
template <typename T> using Matrix1x2 = Matrix<T, 1, 2>;
template <typename T> using Matrix1x3 = Matrix<T, 1, 3>;
template <typename T> using Matrix1x4 = Matrix<T, 1, 4>;
template <typename T> using Matrix1x5 = Matrix<T, 1, 5>;
template <typename T> using Matrix1x6 = Matrix<T, 1, 6>;
template <typename T> using Matrix2x1 = Matrix<T, 2, 1>;
template <typename T> using Matrix2x2 = SquareMatrix<T, 2>;
template <typename T> using Matrix2x3 = Matrix<T, 2, 3>;
template <typename T> using Matrix2x4 = Matrix<T, 2, 4>;
template <typename T> using Matrix2x5 = Matrix<T, 2, 5>;
template <typename T> using Matrix2x6 = Matrix<T, 2, 6>;
template <typename T> using Matrix3x1 = Matrix<T, 3, 1>;
template <typename T> using Matrix3x2 = Matrix<T, 3, 2>;
template <typename T> using Matrix3x3 = SquareMatrix<T, 3>;
template <typename T> using Matrix3x4 = Matrix<T, 3, 4>;
template <typename T> using Matrix3x5 = Matrix<T, 3, 5>;
template <typename T> using Matrix3x6 = Matrix<T, 3, 6>;
template <typename T> using Matrix4x1 = Matrix<T, 4, 1>;
template <typename T> using Matrix4x2 = Matrix<T, 4, 2>;
template <typename T> using Matrix4x3 = Matrix<T, 4, 3>;
template <typename T> using Matrix4x4 = SquareMatrix<T, 4>;
template <typename T> using Matrix4x5 = Matrix<T, 4, 5>;
template <typename T> using Matrix4x6 = Matrix<T, 4, 6>;
template <typename T> using Matrix5x1 = Matrix<T, 5, 1>;
template <typename T> using Matrix5x2 = Matrix<T, 5, 2>;
template <typename T> using Matrix5x3 = Matrix<T, 5, 3>;
template <typename T> using Matrix5x4 = Matrix<T, 5, 4>;
template <typename T> using Matrix5x5 = SquareMatrix<T, 5>;
template <typename T> using Matrix5x6 = Matrix<T, 5, 6>;
template <typename T> using Matrix6x1 = Matrix<T, 6, 1>;
template <typename T> using Matrix6x2 = Matrix<T, 6, 2>;
template <typename T> using Matrix6x3 = Matrix<T, 6, 3>;
template <typename T> using Matrix6x4 = Matrix<T, 6, 4>;
template <typename T> using Matrix6x5 = Matrix<T, 6, 5>;
template <typename T> using Matrix6x6 = SquareMatrix<T, 6>;

template <std::size_t N, std::size_t M> using bMatrix = Matrix<bool, N, M>;
typedef SquareMatrix<bool, 1> bMatrix1x1;
typedef Matrix<bool, 1, 2> bMatrix1x2;
typedef Matrix<bool, 1, 3> bMatrix1x3;
typedef Matrix<bool, 1, 4> bMatrix1x4;
typedef Matrix<bool, 1, 5> bMatrix1x5;
typedef Matrix<bool, 1, 6> bMatrix1x6;
typedef Matrix<bool, 2, 1> bMatrix2x1;
typedef SquareMatrix<bool, 2> bMatrix2x2;
typedef Matrix<bool, 2, 3> bMatrix2x3;
typedef Matrix<bool, 2, 4> bMatrix2x4;
typedef Matrix<bool, 2, 5> bMatrix2x5;
typedef Matrix<bool, 2, 6> bMatrix2x6;
typedef Matrix<bool, 3, 1> bMatrix3x1;
typedef Matrix<bool, 3, 2> bMatrix3x2;
typedef SquareMatrix<bool, 3> bMatrix3x3;
typedef Matrix<bool, 3, 4> bMatrix3x4;
typedef Matrix<bool, 3, 5> bMatrix3x5;
typedef Matrix<bool, 3, 6> bMatrix3x6;
typedef Matrix<bool, 4, 1> bMatrix4x1;
typedef Matrix<bool, 4, 2> bMatrix4x2;
typedef Matrix<bool, 4, 3> bMatrix4x3;
typedef SquareMatrix<bool, 4> bMatrix4x4;
typedef Matrix<bool, 4, 5> bMatrix4x5;
typedef Matrix<bool, 4, 6> bMatrix4x6;
typedef Matrix<bool, 5, 1> bMatrix5x1;
typedef Matrix<bool, 5, 2> bMatrix5x2;
typedef Matrix<bool, 5, 3> bMatrix5x3;
typedef Matrix<bool, 5, 4> bMatrix5x4;
typedef SquareMatrix<bool, 5> bMatrix5x5;
typedef Matrix<bool, 5, 6> bMatrix5x6;
typedef Matrix<bool, 6, 1> bMatrix6x1;
typedef Matrix<bool, 6, 2> bMatrix6x2;
typedef Matrix<bool, 6, 3> bMatrix6x3;
typedef Matrix<bool, 6, 4> bMatrix6x4;
typedef Matrix<bool, 6, 5> bMatrix6x5;
typedef SquareMatrix<bool, 6> bMatrix6x6;

template <std::size_t N, std::size_t M> using iMatrix = Matrix<int, N, M>;
typedef SquareMatrix<int, 1> iMatrix1x1;
typedef Matrix<int, 1, 2> iMatrix1x2;
typedef Matrix<int, 1, 3> iMatrix1x3;
typedef Matrix<int, 1, 4> iMatrix1x4;
typedef Matrix<int, 1, 5> iMatrix1x5;
typedef Matrix<int, 1, 6> iMatrix1x6;
typedef Matrix<int, 2, 1> iMatrix2x1;
typedef SquareMatrix<int, 2> iMatrix2x2;
typedef Matrix<int, 2, 3> iMatrix2x3;
typedef Matrix<int, 2, 4> iMatrix2x4;
typedef Matrix<int, 2, 5> iMatrix2x5;
typedef Matrix<int, 2, 6> iMatrix2x6;
typedef Matrix<int, 3, 1> iMatrix3x1;
typedef Matrix<int, 3, 2> iMatrix3x2;
typedef SquareMatrix<int, 3> iMatrix3x3;
typedef Matrix<int, 3, 4> iMatrix3x4;
typedef Matrix<int, 3, 5> iMatrix3x5;
typedef Matrix<int, 3, 6> iMatrix3x6;
typedef Matrix<int, 4, 1> iMatrix4x1;
typedef Matrix<int, 4, 2> iMatrix4x2;
typedef Matrix<int, 4, 3> iMatrix4x3;
typedef SquareMatrix<int, 4> iMatrix4x4;
typedef Matrix<int, 4, 5> iMatrix4x5;
typedef Matrix<int, 4, 6> iMatrix4x6;
typedef Matrix<int, 5, 1> iMatrix5x1;
typedef Matrix<int, 5, 2> iMatrix5x2;
typedef Matrix<int, 5, 3> iMatrix5x3;
typedef Matrix<int, 5, 4> iMatrix5x4;
typedef SquareMatrix<int, 5> iMatrix5x5;
typedef Matrix<int, 5, 6> iMatrix5x6;
typedef Matrix<int, 6, 1> iMatrix6x1;
typedef Matrix<int, 6, 2> iMatrix6x2;
typedef Matrix<int, 6, 3> iMatrix6x3;
typedef Matrix<int, 6, 4> iMatrix6x4;
typedef Matrix<int, 6, 5> iMatrix6x5;
typedef SquareMatrix<int, 6> iMatrix6x6;

template <std::size_t N, std::size_t M>
using uMatrix = Matrix<unsigned int, N, M>;
typedef SquareMatrix<unsigned int, 1> uMatrix1x1;
typedef Matrix<unsigned int, 1, 2> uMatrix1x2;
typedef Matrix<unsigned int, 1, 3> uMatrix1x3;
typedef Matrix<unsigned int, 1, 4> uMatrix1x4;
typedef Matrix<unsigned int, 1, 5> uMatrix1x5;
typedef Matrix<unsigned int, 1, 6> uMatrix1x6;
typedef Matrix<unsigned int, 2, 1> uMatrix2x1;
typedef SquareMatrix<unsigned int, 2> uMatrix2x2;
typedef Matrix<unsigned int, 2, 3> uMatrix2x3;
typedef Matrix<unsigned int, 2, 4> uMatrix2x4;
typedef Matrix<unsigned int, 2, 5> uMatrix2x5;
typedef Matrix<unsigned int, 2, 6> uMatrix2x6;
typedef Matrix<unsigned int, 3, 1> uMatrix3x1;
typedef Matrix<unsigned int, 3, 2> uMatrix3x2;
typedef SquareMatrix<unsigned int, 3> uMatrix3x3;
typedef Matrix<unsigned int, 3, 4> uMatrix3x4;
typedef Matrix<unsigned int, 3, 5> uMatrix3x5;
typedef Matrix<unsigned int, 3, 6> uMatrix3x6;
typedef Matrix<unsigned int, 4, 1> uMatrix4x1;
typedef Matrix<unsigned int, 4, 2> uMatrix4x2;
typedef Matrix<unsigned int, 4, 3> uMatrix4x3;
typedef SquareMatrix<unsigned int, 4> uMatrix4x4;
typedef Matrix<unsigned int, 4, 5> uMatrix4x5;
typedef Matrix<unsigned int, 4, 6> uMatrix4x6;
typedef Matrix<unsigned int, 5, 1> uMatrix5x1;
typedef Matrix<unsigned int, 5, 2> uMatrix5x2;
typedef Matrix<unsigned int, 5, 3> uMatrix5x3;
typedef Matrix<unsigned int, 5, 4> uMatrix5x4;
typedef SquareMatrix<unsigned int, 5> uMatrix5x5;
typedef Matrix<unsigned int, 5, 6> uMatrix5x6;
typedef Matrix<unsigned int, 6, 1> uMatrix6x1;
typedef Matrix<unsigned int, 6, 2> uMatrix6x2;
typedef Matrix<unsigned int, 6, 3> uMatrix6x3;
typedef Matrix<unsigned int, 6, 4> uMatrix6x4;
typedef Matrix<unsigned int, 6, 5> uMatrix6x5;
typedef SquareMatrix<unsigned int, 6> uMatrix6x6;

template <std::size_t N, std::size_t M> using fMatrix = Matrix<float, N, M>;
typedef SquareMatrix<float, 1> fMatrix1x1;
typedef Matrix<float, 1, 2> fMatrix1x2;
typedef Matrix<float, 1, 3> fMatrix1x3;
typedef Matrix<float, 1, 4> fMatrix1x4;
typedef Matrix<float, 1, 5> fMatrix1x5;
typedef Matrix<float, 1, 6> fMatrix1x6;
typedef Matrix<float, 2, 1> fMatrix2x1;
typedef SquareMatrix<float, 2> fMatrix2x2;
typedef Matrix<float, 2, 3> fMatrix2x3;
typedef Matrix<float, 2, 4> fMatrix2x4;
typedef Matrix<float, 2, 5> fMatrix2x5;
typedef Matrix<float, 2, 6> fMatrix2x6;
typedef Matrix<float, 3, 1> fMatrix3x1;
typedef Matrix<float, 3, 2> fMatrix3x2;
typedef SquareMatrix<float, 3> fMatrix3x3;
typedef Matrix<float, 3, 4> fMatrix3x4;
typedef Matrix<float, 3, 5> fMatrix3x5;
typedef Matrix<float, 3, 6> fMatrix3x6;
typedef Matrix<float, 4, 1> fMatrix4x1;
typedef Matrix<float, 4, 2> fMatrix4x2;
typedef Matrix<float, 4, 3> fMatrix4x3;
typedef SquareMatrix<float, 4> fMatrix4x4;
typedef Matrix<float, 4, 5> fMatrix4x5;
typedef Matrix<float, 4, 6> fMatrix4x6;
typedef Matrix<float, 5, 1> fMatrix5x1;
typedef Matrix<float, 5, 2> fMatrix5x2;
typedef Matrix<float, 5, 3> fMatrix5x3;
typedef Matrix<float, 5, 4> fMatrix5x4;
typedef SquareMatrix<float, 5> fMatrix5x5;
typedef Matrix<float, 5, 6> fMatrix5x6;
typedef Matrix<float, 6, 1> fMatrix6x1;
typedef Matrix<float, 6, 2> fMatrix6x2;
typedef Matrix<float, 6, 3> fMatrix6x3;
typedef Matrix<float, 6, 4> fMatrix6x4;
typedef Matrix<float, 6, 5> fMatrix6x5;
typedef SquareMatrix<float, 6> fMatrix6x6;

template <std::size_t N, std::size_t M> using dMatrix = Matrix<double, N, M>;
typedef SquareMatrix<double, 1> dMatrix1x1;
typedef Matrix<double, 1, 2> dMatrix1x2;
typedef Matrix<double, 1, 3> dMatrix1x3;
typedef Matrix<double, 1, 4> dMatrix1x4;
typedef Matrix<double, 1, 5> dMatrix1x5;
typedef Matrix<double, 1, 6> dMatrix1x6;
typedef Matrix<double, 2, 1> dMatrix2x1;
typedef SquareMatrix<double, 2> dMatrix2x2;
typedef Matrix<double, 2, 3> dMatrix2x3;
typedef Matrix<double, 2, 4> dMatrix2x4;
typedef Matrix<double, 2, 5> dMatrix2x5;
typedef Matrix<double, 2, 6> dMatrix2x6;
typedef Matrix<double, 3, 1> dMatrix3x1;
typedef Matrix<double, 3, 2> dMatrix3x2;
typedef SquareMatrix<double, 3> dMatrix3x3;
typedef Matrix<double, 3, 4> dMatrix3x4;
typedef Matrix<double, 3, 5> dMatrix3x5;
typedef Matrix<double, 3, 6> dMatrix3x6;
typedef Matrix<double, 4, 1> dMatrix4x1;
typedef Matrix<double, 4, 2> dMatrix4x2;
typedef Matrix<double, 4, 3> dMatrix4x3;
typedef SquareMatrix<double, 4> dMatrix4x4;
typedef Matrix<double, 4, 5> dMatrix4x5;
typedef Matrix<double, 4, 6> dMatrix4x6;
typedef Matrix<double, 5, 1> dMatrix5x1;
typedef Matrix<double, 5, 2> dMatrix5x2;
typedef Matrix<double, 5, 3> dMatrix5x3;
typedef Matrix<double, 5, 4> dMatrix5x4;
typedef SquareMatrix<double, 5> dMatrix5x5;
typedef Matrix<double, 5, 6> dMatrix5x6;
typedef Matrix<double, 6, 1> dMatrix6x1;
typedef Matrix<double, 6, 2> dMatrix6x2;
typedef Matrix<double, 6, 3> dMatrix6x3;
typedef Matrix<double, 6, 4> dMatrix6x4;
typedef Matrix<double, 6, 5> dMatrix6x5;
typedef SquareMatrix<double, 6> dMatrix6x6;

template <typename T> using Matrix1 = SquareMatrix<T, 1>;
template <typename T> using Matrix2 = SquareMatrix<T, 2>;
template <typename T> using Matrix3 = SquareMatrix<T, 3>;
template <typename T> using Matrix4 = SquareMatrix<T, 4>;
template <typename T> using Matrix5 = SquareMatrix<T, 5>;
template <typename T> using Matrix6 = SquareMatrix<T, 6>;

typedef SquareMatrix<bool, 1> bMatrix1;
typedef SquareMatrix<bool, 2> bMatrix2;
typedef SquareMatrix<bool, 3> bMatrix3;
typedef SquareMatrix<bool, 4> bMatrix4;
typedef SquareMatrix<bool, 5> bMatrix5;
typedef SquareMatrix<bool, 6> bMatrix6;

typedef SquareMatrix<int, 1> iMatrix1;
typedef SquareMatrix<int, 2> iMatrix2;
typedef SquareMatrix<int, 3> iMatrix3;
typedef SquareMatrix<int, 4> iMatrix4;
typedef SquareMatrix<int, 5> iMatrix5;
typedef SquareMatrix<int, 6> iMatrix6;

typedef SquareMatrix<unsigned int, 1> uMatrix1;
typedef SquareMatrix<unsigned int, 2> uMatrix2;
typedef SquareMatrix<unsigned int, 3> uMatrix3;
typedef SquareMatrix<unsigned int, 4> uMatrix4;
typedef SquareMatrix<unsigned int, 5> uMatrix5;
typedef SquareMatrix<unsigned int, 6> uMatrix6;

typedef SquareMatrix<float, 1> fMatrix1;
typedef SquareMatrix<float, 2> fMatrix2;
typedef SquareMatrix<float, 3> fMatrix3;
typedef SquareMatrix<float, 4> fMatrix4;
typedef SquareMatrix<float, 5> fMatrix5;
typedef SquareMatrix<float, 6> fMatrix6;

typedef SquareMatrix<double, 1> dMatrix1;
typedef SquareMatrix<double, 2> dMatrix2;
typedef SquareMatrix<double, 3> dMatrix3;
typedef SquareMatrix<double, 4> dMatrix4;
typedef SquareMatrix<double, 5> dMatrix5;
typedef SquareMatrix<double, 6> dMatrix6;

template <typename T> using Vector1 = Vector<T, 1>;
template <typename T> using Vector2 = Vector<T, 2>;
template <typename T> using Vector3 = Vector<T, 3>;
template <typename T> using Vector4 = Vector<T, 4>;
template <typename T> using Vector5 = Vector<T, 5>;
template <typename T> using Vector6 = Vector<T, 6>;

typedef Vector<bool, 1> bVector1;
typedef Vector<bool, 2> bVector2;
typedef Vector<bool, 3> bVector3;
typedef Vector<bool, 4> bVector4;
typedef Vector<bool, 5> bVector5;
typedef Vector<bool, 6> bVector6;

typedef Vector<int, 1> iVector1;
typedef Vector<int, 2> iVector2;
typedef Vector<int, 3> iVector3;
typedef Vector<int, 4> iVector4;
typedef Vector<int, 5> iVector5;
typedef Vector<int, 6> iVector6;

typedef Vector<unsigned int, 1> uVector1;
typedef Vector<unsigned int, 2> uVector2;
typedef Vector<unsigned int, 3> uVector3;
typedef Vector<unsigned int, 4> uVector4;
typedef Vector<unsigned int, 5> uVector5;
typedef Vector<unsigned int, 6> uVector6;

typedef Vector<float, 1> fVector1;
typedef Vector<float, 2> fVector2;
typedef Vector<float, 3> fVector3;
typedef Vector<float, 4> fVector4;
typedef Vector<float, 5> fVector5;
typedef Vector<float, 6> fVector6;

typedef Vector<double, 1> dVector1;
typedef Vector<double, 2> dVector2;
typedef Vector<double, 3> dVector3;
typedef Vector<double, 4> dVector4;
typedef Vector<double, 5> dVector5;
typedef Vector<double, 6> dVector6;
} // namespace physim::linalg

template <typename T, typename Char>
struct fmt::formatter<
    T, Char,
    typename std::enable_if<
        std::is_base_of<physim::linalg::detail::MatrixBase, T>::value,
        void>::type> {

  std::basic_string<Char> spec;
  Char prefix = '{', postfix = '}';

  template <typename OutputIt> OutputIt write_delimiter(OutputIt out) {
    *out++ = ',';
    *out++ = ' ';
    return out;
  }
  template <typename OutputIt> OutputIt copy(Char ch, OutputIt out) {
    *out++ = ch;
    return out;
  }

  template <typename ParseContext>
  constexpr auto parse(ParseContext &ctx) -> decltype(ctx.begin()) {
    auto it = ctx.begin();
    auto end = ctx.end();
    while (it != end && (*it != '}')) {
      spec += *it++;
    }
    return it;
  }

  template <typename FormatContext>
  typename FormatContext::iterator format(const T &values, FormatContext &ctx) {
    auto out = copy(prefix, ctx.out());
    size_t i = 0, row = values.cols();
    auto it = values.begin();
    auto end = values.end();
    for (; it != end; ++it) {
      if ((i % row) == 0 && i != 0)
        out = copy(postfix, out);
      if (i > 0)
        out = write_delimiter(out);
      if ((i % row) == 0)
        out = copy(prefix, out);
      out = format_to(out, "{:" + spec + "}", *it);
      ++i;
    }
    return copy(postfix, copy(postfix, out));
  }
};

#endif /* end of include guard: LINALG_HPP_ */
