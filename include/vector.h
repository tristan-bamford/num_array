#ifndef TB_MATH_NUM_ARRAY_VECTOR_H
#define TB_MATH_NUM_ARRAY_VECTOR_H

#include "num_array.h"
#include <cmath>

namespace tb::math {
  
  // Vector aliases

  template<Number T>
    using vec2 = num_array<T, 2>;
  template<Number T>
    using vec3 = num_array<T, 3>;
  template<Number T>
    using vec4 = num_array<T, 4>;

  using vec2f = vec2<float>;
  using vec3f = vec3<float>;
  using vec4f = vec4<float>;

  using vec2d = vec2<double>;
  using vec3d = vec3<double>;
  using vec4d = vec4<double>;

  using vec2i = vec2<int>;
  using vec3i = vec3<int>;
  using vec4i = vec4<int>;

  using vec2l = vec2<long>;
  using vec3l = vec3<long>;
  using vec4l = vec4<long>;

  using vec2u = vec2<unsigned>;
  using vec3u = vec3<unsigned>;
  using vec4u = vec4<unsigned>;

  using vec2ul = vec2<unsigned long>;
  using vec3ul = vec3<unsigned long>;
  using vec4ul = vec4<unsigned long>;
}

namespace tb::math {

  // Outer Product
  // Returns a num_array (matrix) that is the outer product of two 
  // num_array (vectors).
  template<Number T1, Number T2, std::size_t N1, std::size_t N2>
    [[nodiscard]] constexpr auto
    outer_product(const num_array<T1, N1>& v, const num_array<T2, N2>& w)
      requires std::common_with<T1, T2>
    {
      using R = typename std::common_type<T1, T2>::type;
      num_array<R, N1, N2> result;
      for (std::size_t m = 0; m < N1; ++m) {
        result[m] = w * static_cast<T2>(v[m]);
      }
      return result;
    }

  // Dot Product
  // Returns the dot(scalar) product of two vectors
  template<Number T1, Number T2, std::size_t N>
    [[nodiscard]] constexpr auto
    dot_product(const num_array<T1, N>& v, const num_array<T2, N>& w)
    {
      typename std::common_type<T1, T2>::type result{0};
      for (std::size_t i = 0; i < N; ++i) {
        result += w[i] * v[i];
      }
      return result;    
    }

  // Cross Product
  // Returns the value of the cross product, v × w, between two three-dimensional 
  // vectors.
  template<Number T1, Number T2, Number R = std::common_type<T1, T2>::type> 
    [[nodiscard]] constexpr num_array<R, 3>
    cross_product(const num_array<T1, 3>& v, const num_array<T2, 3>& w)
    {
      return { v[1] * w[2] - v[2] * w[1], 
               v[2] * w[0] - v[0] * w[2], 
               v[0] * w[1] - v[1] * w[0] };
    }

  // Triple Product
  // Returns the value of the scalar triple product, u · (v × w), where u, v and 
  // w are three-dimensional vectors.
  template<Number T1, Number T2, Number T3> 
    [[nodiscard]] constexpr auto
    triple_product(const num_array<T1, 3>& u, const num_array<T2, 3>& v, 
                   const num_array<T3, 3>& w)
    {
      return dot_product(u, cross_product(v, w));
    }

  // Magnitude
  // Returns the magnitude of a vector
  template<Number T, std::size_t N>
    [[nodiscard]] constexpr auto
    magnitude(const num_array<T, N>& v)
    {
      return std::sqrt(dot_product(v, v)); // NOTE: std::sqrt is not constexpr
    }
  
  // Unit Vector
  // Returns the unit vector (normalization) of a vector.
  // NOTE: Division by zero when v = 0.
  template<Number T, std::size_t N>
    [[nodiscard]] constexpr auto
    dir(const num_array<T, N>& v)
    {
      return v / magnitude(v); // NOTE: magnitude() is not constexpr
    }

  // Projection
  // Returns the projection of w onto v.
  // NOTE: Division by zero when v = 0
  template<Number T1, Number T2, std::size_t N>
    [[nodiscard]] constexpr auto
    projection(const num_array<T1, N>& v, const num_array<T2, N>& w)
    {
      return v * (dot_product(v, w) / dot_product(v, v));
    }
}
#endif//TB_MATH_NUM_ARRAY_VECTOR_H