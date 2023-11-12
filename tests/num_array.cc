#include "tests.h"
#include "../src/matrix.h"

using tb::math::num_array, tb::math::Number;

template<Number T, std::size_t M, std::size_t... N>
  void test_sub_array_base()
  {
    using Num_array = num_array<T, M, N...>;
    constexpr Num_array x(0.0);
    assert(x.size() == M);
    assert(x.order() == sizeof...(N) + 1);
  }

template<Number T, std::size_t M, std::size_t... N>
constexpr auto test_constructors()
{
  using Num_array = num_array<T, M, N...>;
  constexpr T value = 3;
  
  Num_array t1;        // uninitialized construction
  t1 = value;          // value assignment
  assert(t1 == value);

  Num_array t2(value); // value construction
  assert(t1 == t2);

  Num_array t3(t2);    // copy construction
  assert(t1 == t3);

  Num_array t4(t3);    // copy assignment
  assert(t1 == t4);

  constexpr Num_array u1(value); // constexpr value construction
  static_assert(u1 == value);

  constexpr Num_array u2(u1);    // constexpr copy construction
  static_assert(u2 == value);

  return u2;
}

template<Number T, std::size_t M, std::size_t... N>
constexpr auto test_accessors()
{
  using Num_array = num_array<T, M, N...>;
  constexpr T value = 3;

  constexpr Num_array t1(value);
  assert(t1[0] == value);
  
  constexpr auto x = value + 1;
  Num_array t2(t1), t3(t2);
  t2[0] = x;
  assert(t2[0] == x);

  for (std::size_t i = 0; i < t3.size(); ++i) { assert(t3[i] == value); }
}


template<Number T, std::size_t M, std::size_t... N>
  constexpr auto test_operators()
  {
    using Num_array = num_array<T, M, N...>;
    Num_array A;
    for (std::size_t i = 0; i < A.size(); ++i) A[i] = i;

    auto B(A + A);
    assert(A * 2 == B);
    assert(B / 2 == A);
    assert(B - A == A);
    auto C(A - A);
    assert(C == 0);
    //assert(-A + A == C);
    assert(C + 1 == 1);
    assert(C - 1 == -1);
  }

template<Number T, std::size_t M, std::size_t... N>
  constexpr auto general_tests()
  {
    test_sub_array_base<T, M, N...>();

    test_constructors<T, M, N...>();
    test_accessors<T, M, N...>();
    test_operators<T, M, N...>();
  }

template<Number T>
  constexpr auto test_type()
  {
    general_tests<T, 1>();
    general_tests<T, 4>();
    general_tests<T, 4, 10>();
    //general_tests<T, 100, 4, 3>();
  }

int main()
{
  constexpr num_array<int,0> x; // empty num_array

  test_type<int>();
  test_type<unsigned>();
  test_type<float>();
  test_type<double>();

  return EXIT_SUCCESS;
}