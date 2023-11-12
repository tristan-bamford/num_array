#include "../src/vector.h"

using tb::math::Number;

template<Number T>
  void test_outer_product()
  {
    constexpr tb::math::num_array<T, 2> w = { 1, 2 };
    constexpr tb::math::num_array<T, 3> v = { 3, 4, 5 };
    constexpr tb::math::num_array<T, 3, 2> vw = {{3,6},{4,8},{5,10}};
    constexpr tb::math::num_array<T, 2, 3> wv = {{3,4,5},{6,8,10}};
    assert(outer_product(v, w) == vw);
    assert(outer_product(w, v) == wv);
  }


template<Number T>
  void test_dot_product()
  {
    constexpr tb::math::num_array<T, 3> v1 = { 1, 0, 0 }, v2 = { 0, 1, 0 };
    assert(dot_product(v1, v2) == 0);
  }

template<Number T>
  void test_magnitude()
  {
    constexpr tb::math::num_array<T, 2> v = {3,4};
    assert(magnitude(v) == 5);
  }

template<Number T>
  void test_type()
  {
    test_outer_product<T>();
    test_dot_product<T>();
    test_magnitude<T>();
  }

int main()
{
  test_type<double>();
  test_type<float>();
  test_type<int>();

  return EXIT_SUCCESS;
}