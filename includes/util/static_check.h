#ifndef STATIC_CHECK_H
#define STATIC_CHECK_H

// Implementations from cppreference.com

namespace util {

template <class T> struct remove_cv { typedef T type; };
template <class T> struct remove_cv<const T> { typedef T type; };
template <class T> struct remove_cv<volatile T> { typedef T type; };
template <class T> struct remove_cv<const volatile T> { typedef T type; };

template <class T> struct remove_const { typedef T type; };
template <class T> struct remove_const<const T> { typedef T type; };

template <class T> struct remove_volatile { typedef T type; };
template <class T> struct remove_volatile<volatile T> { typedef T type; };

namespace detail {

template <class T> struct type_identity {
  using type = T;
}; // or use std::type_identity (since C++20)

template <class T> // Note that `cv void&` is a substitution failure
auto try_add_lvalue_reference(int) -> type_identity<T &>;
template <class T> // Handle T = cv void case
auto try_add_lvalue_reference(...) -> type_identity<T>;

template <class T> auto try_add_rvalue_reference(int) -> type_identity<T &&>;
template <class T> auto try_add_rvalue_reference(...) -> type_identity<T>;

} // namespace detail

template <class T>
struct add_lvalue_reference : decltype(detail::try_add_lvalue_reference<T>(0)) {
};

template <class T>
struct add_rvalue_reference : decltype(detail::try_add_rvalue_reference<T>(0)) {
};

template <class T> typename add_rvalue_reference<T>::type declval() noexcept;

template <class T, T v> struct integral_constant {
  static constexpr T value = v;
  using value_type = T;
  using type = integral_constant; // using injected-class-name
  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept { return value; }
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template <class T, class U> struct is_same : false_type {};

template <class T> struct is_same<T, T> : true_type {};

template <class T>
struct is_void : is_same<void, typename remove_cv<T>::type> {};

namespace detail {

template <class T>
auto test_returnable(int)
    -> decltype(void(static_cast<T (*)()>(nullptr)), true_type{});
template <class> auto test_returnable(...) -> false_type;

template <class From, class To>
auto test_implicitly_convertible(int)
    -> decltype(void(declval<void (&)(To)>()(declval<From>())), true_type{});
template <class, class> auto test_implicitly_convertible(...) -> false_type;

} // namespace detail

template <class From, class To>
struct is_convertible
    : integral_constant<
          bool, (decltype(detail::test_returnable<To>(0))::value &&decltype(
                    detail::test_implicitly_convertible<From, To>(0))::value) ||
                    (is_void<From>::value && is_void<To>::value)> {};

} // namespace util

#endif
