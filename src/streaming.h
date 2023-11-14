#ifndef TB_MATH_NUM_ARRAY_STREAMING_H
#define TB_MATH_NUM_ARRAY_STREAMING_H

#include <iostream>
#include "num_array.h"

namespace tb::math {
  template<typename T, std::size_t M, std::size_t... N>
    std::ostream&
    operator<<(std::ostream& os, const num_array<T, M, N...>& array)
    {
      os << "{ ";
      for(std::size_t i = 0; i < M; ++i) {
        os << array[i] << (i < (M-1) ? ", " : "");
      } 

      return os << " }";
    }
}
#endif//TB_MATH_NUM_ARRAY_STREAMING_H