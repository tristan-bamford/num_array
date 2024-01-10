#ifndef TB_MATH_NUM_ARRAY_H
#define TB_MATH_NUM_ARRAY_H

#include <concepts>
#include <algorithm>
#include <cassert>

namespace tb::math {

  template<typename T>
    concept Index_type = std::convertible_to<T, std::size_t>;

  // A Number is an element of a set for which the operations +,-,* and / are
  // defined.
  // NOTE: The requirement for the definition of additive and multiplicative 
  // identities is not necessary. However, in the case of the multiplicative 
  // identity, optimizations can be made in some division functions if this 
  // value is known.
  template<typename T>
    concept Number = requires(T x)
    {
      x = 0; // additive identity
      x = 1; // multiplicative identity

      x += x; // field operations
      x -= x;
      x *= x;
      x /= x;

      -x; // negation
    };
}

namespace tb::math {

  // Base class for num_array representing its template structure.
  template<Number T, std::size_t M, std::size_t... N>
    class num_array_base {
      static constexpr std::size_t extents_[] = { M, N... };
    public:
      static consteval auto size() { return M; }// number of "rows"
      static consteval auto order() { return sizeof...(N) + 1; }// "rank"
      static consteval auto extent(std::size_t i) { return extents_[i]; }
      //number of total elements in the array...
      //static consteval auto n_elements() { return (M * ... * N); }
      //static consteval bool empty() { return n_elements() == 0; }      
    };

  // Class template for the num_array family of classes.
  template<Number T, std::size_t M, std::size_t... N>
    class num_array : public num_array_base<T, M, N...> {
    public:
      using sub_array       = num_array<T, N...>;
      using element_type    = sub_array::element_type;
      using value_type      = sub_array;
      using size_type       = sub_array::size_type;
      using iterator        = sub_array*;
      using const_iterator  = const sub_array*;

      constexpr num_array() { } // uninitialized num_array
      template<Number U>
        constexpr num_array(const num_array<U, M, N...>& x)
          requires std::common_with<T, U>;
      constexpr num_array(const T& x);
      constexpr num_array(const std::initializer_list<sub_array>& init_list);

      constexpr auto& operator[](size_type i) const noexcept { return data_[i]; }
      constexpr auto& operator[](size_type i)       noexcept { return data_[i]; }

      template<Index_type Index, Index_type... Indices>
        constexpr auto& operator()(Index i, Indices... j) const noexcept
          requires (sizeof...(Indices) + 1 == num_array_base<T, M, N...>::order())
        { return data_[i](j...); }

      template<Index_type Index, Index_type... Indices>
        constexpr auto& operator()(Index i, Indices... j) noexcept
          requires (sizeof...(Indices) + 1 == num_array_base<T, M, N...>::order())
        { return data_[i](j...); }

      template<Index_type Index, Index_type... Indices>
        constexpr auto& at(Index i, Indices... j) const
          requires (sizeof...(Indices) + 1 == num_array_base<T, M, N...>::order())
        { assert(i < this->size()); return data_[i](j...); }

      template<Index_type Index, Index_type... Indices>
        constexpr auto& at(Index i, Indices... j) 
          requires (sizeof...(Indices) + 1 == num_array_base<T, M, N...>::order())
        { assert(i < this->size()); return data_[i](j...); }

      constexpr auto& apply(auto func);
      constexpr auto& apply(const num_array&, auto func);

      // Iterators

      constexpr const_iterator begin()  const noexcept { return data_; }
      constexpr iterator       begin()        noexcept { return data_; }

      constexpr const_iterator end()    const noexcept { return data_ + this->size(); }
      constexpr iterator       end()          noexcept { return data_ + this->size(); }

      constexpr const_iterator cbegin() const noexcept { return begin(); }
      constexpr const_iterator cend()   const noexcept { return end(); }

      // Arithmetic operations

      constexpr auto& operator+=(const T& rhs)
      { return apply([&](sub_array& x){ x += rhs; }); }
      constexpr auto& operator-=(const T& rhs)
      { return apply([&](sub_array& x){ x -= rhs; }); }
      constexpr auto& operator*=(const T& rhs)
      { return apply([&](sub_array& x){ x *= rhs; }); }
      constexpr auto& operator/=(const T& rhs)
      { return apply([&](sub_array& x){ x /= rhs; }); }

      constexpr auto& operator+=(const num_array& rhs)
      { return apply(rhs, [](sub_array& x, const sub_array& y){ x += y; }); }
      constexpr auto& operator-=(const num_array& rhs)
      { return apply(rhs, [](sub_array& x, const sub_array& y){ x -= y; }); }

    private:
      sub_array data_[M];
    };
  
  template<Number T, std::size_t M, std::size_t... N>
    template<Number U>
      constexpr 
      num_array<T, M, N...>::num_array(const num_array<U, M, N...>& x)
        requires std::common_with<T, U>
    {
      std::copy(x.begin(), x.end(), begin());
    }

  template<Number T, std::size_t M, std::size_t... N>
    constexpr 
    num_array<T, M, N...>::num_array(const T& x)
    {
      std::fill_n(data_, this->size(), x);
    }

  template<Number T, std::size_t M, std::size_t... N>
    constexpr 
    num_array<T, M, N...>::num_array(const std::initializer_list<sub_array>& init_list)
    {
      assert(init_list.size() == M);
      std::copy(init_list.begin(), init_list.end(), begin());
    }

  template<Number T, std::size_t M, std::size_t... N>
    constexpr auto&
    num_array<T, M, N...>::apply(auto func)
    {
      std::for_each_n(data_, this->size(), func);
      return *this;
    }

  template<Number T, std::size_t M, std::size_t... N>
    constexpr auto&
    num_array<T, M, N...>::apply(const num_array& x, auto func)
    {
      auto j = x.begin();
      for (auto i = begin(); i != end(); ++i, ++j) func(*i, *j);
      return *this;
    }
  
  // Specialization of the num_array class for "one-dimensional" arrays 
  // (vectors). 
  template<Number T, std::size_t N>
    class num_array<T, N> : public num_array_base<T, N> {
    public:
      using element_type    = std::remove_const<T>::type;
      using value_type      = element_type;
      using size_type       = std::size_t;
      using pointer         = value_type*;
      using const_pointer   = const value_type*;
      using iterator        = pointer;
      using const_iterator  = const_pointer;

      constexpr num_array() { } // uninitialized num_array;
      template<Number U>
        constexpr num_array(const num_array<U, N>& x)
          requires std::common_with<T, U>;
      constexpr num_array(const T& x);
      constexpr num_array(const std::initializer_list<T>&);

      constexpr auto& operator[](size_type i) const noexcept { return data_[i]; }
      constexpr auto& operator[](size_type i)       noexcept { return data_[i]; }

      constexpr auto& operator()(size_type i) const noexcept { return data_[i]; }
      constexpr auto& operator()(size_type i)       noexcept { return data_[i]; }

      constexpr auto& at(size_type i) const { assert(i < this->size()); return data_[i]; }
      constexpr auto& at(size_type i)       { assert(i < this->size()); return data_[i]; }

      constexpr auto& apply(auto func);
      constexpr auto& apply(const num_array&, auto func);

      // Iterators

      constexpr const_iterator begin()  const noexcept { return data_; }
      constexpr iterator       begin()        noexcept { return data_; }

      constexpr const_iterator end()    const noexcept { return data_ + this->size(); }
      constexpr iterator       end()          noexcept { return data_ + this->size(); }

      constexpr const_iterator cbegin() const noexcept { return begin(); }
      constexpr const_iterator cend()   const noexcept { return end(); }

      // Arithmetic operations

      constexpr auto& operator+=(const T& rhs)
      { return apply([&](T& x){ x += rhs; }); }
      constexpr auto& operator-=(const T& rhs)
      { return apply([&](T& x){ x -= rhs; }); }
      constexpr auto& operator*=(const T& rhs)
      { return apply([&](T& x){ x *= rhs; }); }
      constexpr auto& operator/=(const T& rhs) 
      { return apply([&](T& x){ x /= rhs; }); }

      constexpr auto& operator+=(const num_array& rhs)
      { return apply(rhs, [](T& x, const T& y){ x += y; }); }
      constexpr auto& operator-=(const num_array& rhs)
      { return apply(rhs, [](T& x, const T& y){ x -= y; }); }
      
    private:
      T data_[N];
    };
  
  template<Number T, std::size_t N>
    template<Number U> //requires std::common_with<T, U>
      constexpr 
      num_array<T, N>::num_array(const num_array<U, N>& x)
        requires std::common_with<T, U>
    {
      std::copy(x.begin(), x.end(), begin());
    }

  template<Number T, std::size_t N>
    constexpr 
    num_array<T, N>::num_array(const T& value)
    {
      std::fill_n(data_, this->size(), value);
    }

  // Construct a num_array with an initializer list
  template<Number T, std::size_t N>
    constexpr 
    num_array<T, N>::num_array(const std::initializer_list<T>& init_list)
    {
      //if (init_list.size() == 0) {
        //std::fill(begin(), end(), static_cast<T>(0));
      
      if (init_list.size() == 1) {
        std::fill(begin(), end(),(*init_list.begin()));
      } else if (init_list.size() == N) {
        std::copy(init_list.begin(), init_list.end(), begin());
      } else {
        throw std::invalid_argument("Incorrect number of arguments");
      }
    }
  
  template<Number T, std::size_t N>
    constexpr auto&
    num_array<T, N>::apply(auto func)
    {
      std::for_each_n(data_, this->size(), func);
      return *this;
    }

  template<Number T, std::size_t N>
    constexpr auto&
    num_array<T, N>::apply(const num_array& x, auto func)
    {
      auto j = x.begin();
      auto i = begin(); 
      while (i != end()) func(*i++, *j++);
      return *this;
    }
  
  // Operations
  
  // Scalar addition
  template<Number T, Number U, std::size_t M, std::size_t... N>
    constexpr auto
    operator+(const num_array<T, M, N...>& lhs, const U& rhs)
      requires std::common_with<T, U>    
    {
      using R = typename std::common_type<T, U>::type;
      return num_array<R, M, N...>(lhs) += rhs;
    }

  // Scalar subtraction
  template<Number T, Number U, std::size_t M, std::size_t... N>
    constexpr auto
    operator-(const num_array<T, M, N...>& lhs, const U& rhs)
      requires std::common_with<T, U>
    {
      using R = typename std::common_type<T, U>::type;
      return num_array<R, M, N...>(lhs) -= rhs;
    }

  // Scalar multiplication
  template<Number T, Number U, std::size_t M, std::size_t... N>
    constexpr auto
    operator*(const num_array<T, M, N...>& lhs, const U& rhs)
      requires std::common_with<T, U>
    {
      using R = typename std::common_type<T, U>::type;
      return num_array<R, M, N...>(lhs) *= rhs;
    }
  
  // Commutative scalar multiplication
  template<Number T, Number U, std::size_t M, std::size_t... N>
    constexpr auto
    operator*(const U& lhs, const num_array<T, M, N...>& rhs)
      requires std::common_with<T, U>
    {
      using R = typename std::common_type<T, U>::type;
      return num_array<R, M, N...>(rhs) *= lhs;
    }

  // Negation
  template<Number T, std::size_t M, std::size_t... N>
    constexpr auto
    operator-(const num_array<T, M, N...>& rhs)
    {
      return num_array<T, M, N...>(rhs).apply([](T& x){ x = -x; });
    }

  // Scalar division
  template<Number T, Number U, std::size_t M, std::size_t... N>
    constexpr auto
    operator/(const num_array<T, M, N...>& lhs, const U& rhs)
      requires std::common_with<T, U>
    {
      using R = typename std::common_type<T, U>::type;
      return num_array<R, M, N...>(lhs) /= rhs;
    }

  // Element-wise addition
  template<Number T, Number U, std::size_t M, std::size_t... N>
    constexpr auto
    operator+(const num_array<T, M, N...>& lhs, 
              const num_array<U, M, N...>& rhs)
      requires std::common_with<T, U>
    {
      using R = std::common_type<T, U>::type;
      return num_array<R, M, N...>(lhs) += rhs;
    }

  // Element-wise Subtraction
  template<Number T, Number U, std::size_t M, std::size_t... N>
    constexpr auto
    operator-(const num_array<T, M, N...>& lhs, 
              const num_array<U, M, N...>& rhs)
      requires std::common_with<T, U>
    {
      using R = std::common_type<T, U>::type;
      return num_array<R, M, N...>(lhs) -= rhs;
    }

  // Element-wise comparisons
  template<Number T, Number U, std::size_t M, std::size_t... N>
    constexpr bool
    operator==(const num_array<T, M, N...>& lhs, 
               const num_array<U, M, N...>& rhs)
      requires std::common_with<T, U>
    {
      for (std::size_t i = 0; i < M; ++i) {
        if (lhs[i] != rhs[i]) { return false; }
      }
      return true;
    }

  template<Number T, Number U, std::size_t M, std::size_t... N>
    constexpr bool
    operator!=(const num_array<T, M, N...>& lhs, 
               const num_array<U, M, N...>& rhs)
      requires std::common_with<T, U>
    {
      return !(lhs == rhs);
    }

  // Scalar comparisons
  template<Number T, Number U, std::size_t M, std::size_t... N>
    constexpr bool
    operator==(const num_array<T, M, N...>& lhs, const U& rhs)
      requires std::common_with<T, U>
    {
      for (std::size_t i = 0; i < M; ++i) {
        if (lhs[i] != rhs) { return false; }
      }
      return true;
    }

  template<Number T, Number U, std::size_t M, std::size_t... N>
    constexpr bool
    operator!=(const num_array<T, M, N...>& lhs, const U& rhs)
      requires std::common_with<T, U>
    {
      return !(lhs == rhs);
    }

  // Returns a num_array where elements are the absolute value of the 
  // corresponding elements of v
  template<Number T, std::size_t M, std::size_t... N>
    constexpr auto
    abs(const num_array<T, M, N...>& v)
    {
      num_array<T, M, N...> result;
      result.apply(v, [](T& x, const T& y){ x = std::abs(y); });
      return result;
    }
} // namespace tb::math
#endif//TB_MATH_NUM_ARRAY_H
