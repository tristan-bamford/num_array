#ifndef TB_MATH_NUM_ARRAY_MATRIX_H
#define TB_MATH_NUM_ARRAY_MATRIX_H

#include "num_array.h"
#include "vector.h" // dot_product()

namespace tb::math {

  template<Number T>
    using mat2 = num_array<T, 2, 2>;

  using mat2f = mat2<float>;
  // ...
}

namespace tb::math {

  template<Number T, std::size_t M, std::size_t N>
    [[nodiscard]] constexpr auto
    transpose(const num_array<T, M, N>& x)
    {
      num_array<T, N, M> result;
      for (std::size_t i = 0; i < M; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
          result(j, i) = x(i, j);
        }
      }
      return result;
    }
  
  // Determinate of a 2x2 matrix
  template<Number T>
    [[nodiscard]] constexpr auto
    det(const num_array<T, 2, 2>& x) 
    {
      return (x(0,0) * x(1,1)) - (x(0,1) * x(1,0));
    }

  template<Number T1, Number T2, std::size_t M, std::size_t N, std::size_t P,
           Number R = std::common_type<T1, T2>::type>
    [[nodiscard]] constexpr auto
    matrix_product(const num_array<T1, M, N>& lhs, const num_array<T2, N, P>& rhs)
    {
      //using R = typename std::common_type<T, U>::type;
      num_array<R, M, P> result;
      for (std::size_t i = 0; i < M; ++i) {
        for (std::size_t j = 0; j < P; ++j) {
          R sum = 0;
          for (std::size_t k = 0; k < N; ++k) {
            sum += lhs(i, k) * rhs(k, j);
          }
          result(i, j) = sum;
        }
      }
      return result;
    }
  
  template<Number T1, Number T2, std::size_t M, std::size_t N,
           Number R = std::common_type<T1, T2>::type>
    [[nodiscard]] constexpr auto
    vector_matrix_product(const num_array<T1, M>& lhs, 
                          const num_array<T2, M, N>& rhs)
    {
      num_array<R, N> result;
      for (std::size_t j = 0; j < N; ++j) {
        R sum = 0;
        for (std::size_t k = 0; k < M; ++k) {
          sum += lhs[k] * rhs(k, j);
        }
        result[j] = sum;
      }
      return result;
    }

  template<Number T1, Number T2, std::size_t M, std::size_t N,
           Number R = std::common_type<T1, T2>::type>
    [[nodiscard]] constexpr auto
    matrix_vector_product(const num_array<T1, M, N>& lhs,
                          const num_array<T2, N>& rhs)
    {
      num_array<R, M> result;
      for (std::size_t i = 0; i < M; ++i) {
        result[i] = dot_product(lhs[i], rhs);
      }
      return result;
    }
  
  // operator* override for matrix multiplication
  template<Number T1, Number T2, std::size_t M, std::size_t N, std::size_t P>
    [[nodiscard]] constexpr auto
    operator*(const num_array<T1, M, N>& lhs, const num_array<T2, N, P>& rhs)
    {
      return matrix_product(lhs, rhs);
    }

  template<Number T1, Number T2, std::size_t M, std::size_t N>
    [[nodiscard]] constexpr auto
    operator*(const num_array<T1, M>& lhs, const num_array<T2, M, N>& rhs)
    {
      return vector_matrix_product(lhs, rhs);
    }

  template<Number T1, Number T2, std::size_t M, std::size_t N>
    [[nodiscard]] constexpr auto
    operator*(const num_array<T1, M, N>& lhs, const num_array<T2, N>& rhs)
    {
      return matrix_vector_product(lhs, rhs);
    }
}
#endif//TB_MATH_NUM_ARRAY_MATRIX_H