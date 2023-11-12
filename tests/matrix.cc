#include "../src/matrix.h"

int main()
{
  constexpr tb::math::num_array<float, 2, 3> A({{1,2,3}, {4,5,6}});

  constexpr auto B = transpose(A);

  const auto x1 = det(A * transpose(A));

  const auto x = B * A;

  constexpr tb::math::num_array<float, 3> v = { 1, 2, 3 };

  constexpr auto y = A * v;
}