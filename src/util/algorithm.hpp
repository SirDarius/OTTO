#pragma once

#include <algorithm>
#include <initializer_list>
#include <functional>
#include <string>
#include "util/type_traits.hpp"

namespace top1 {
  /** cldoc:begin-category(algorithms) */

  /// Joins a sequence of strings, separating them using `js`
  template<class StrIterator> // Models InputIterator<std::string>
  auto join_strings(StrIterator b, StrIterator e, std::string_view js = ", ") -> std::string {
    std::string result;
    std::for_each(b, e, [&] (auto&& s) {
        if (!result.empty()) {
          result.append(js);
        }
        result.append(s);
      });
    return result;
  }

  namespace detail {
    template<class Func, int... ns>
    constexpr auto generate_sequence_impl(std::integer_sequence<int, ns...>&&, Func&& gen) {
      #pragma clang diagnostic push
      #pragma clang diagnostic ignored "-Wmissing-braces"
      return std::array{std::invoke(std::forward<Func>(gen), ns)...};
      #pragma clang diagnostic pop
    }
  }

  template<int n, class Func>
  constexpr auto generate_sequence(Func&& gen) {
    auto intseq = std::make_integer_sequence<int, n>();
    return detail::generate_sequence_impl(std::move(intseq), std::forward<Func>(gen));
  }


  template<typename InputIt, typename Size, typename F>
    auto for_each_n(InputIt&& first, Size n, F&& f) ->
    std::enable_if_t<
      is_iterator_v<InputIt, typename std::iterator_traits<InputIt>::value_type,
        std::input_iterator_tag>
      && std::is_invocable_v<F,
        const typename std::iterator_traits<InputIt>::reference>, InputIt> {
    for (Size i = 0; i < n; ++first, (void) ++i) {
      std::invoke(std::forward<F>(f), *first, i);
    }
    return first;
  }

  /// `for_each` with access to an index value
  ///
  /// For each item in range `[first, last)`, invoke `f` with args
  /// `*iter, i` where `iter` is the current iterator, and `i` is
  /// an incrementing value, starting at zero. Use this instead of
  /// raw indexed loops wherever possible.
  /// 
  /// @first Input iterator to the begining of the range
  /// @last Input iterator to the end of the range
  /// @f Must be invocable with arguments `value_type`, `std::size_t`
  /// @return The number of iterations performed
  template<typename InputIt, typename F>
    auto indexed_for(InputIt&& first, InputIt&& last, F&& f) ->
    std::enable_if_t<
      is_iterator_v<InputIt, typename std::iterator_traits<InputIt>::value_type,
        std::input_iterator_tag>
      && std::is_invocable_v<F,
        const typename std::iterator_traits<InputIt>::reference, std::size_t>,
      std::size_t> {
    std::size_t i;
    std::for_each(std::forward<InputIt>(first), std::forward<InputIt>(last),
        [&] (auto&& a) {
          std::invoke(std::forward<F>(f), a, i);
          i++;
        });
    return i;
  }

  /// `for_each_n` with access to an index value
  ///
  /// for `n` iterations, invoke `f` with args `*iter, i`
  /// where `iter` is the current iterator starting with `first`,
  /// and `i` is an incrementing value, starting at zero.
  /// Use this instead of raw indexed loops wherever possible.
  ///
  /// @first Input iterator to the begining of the range
  /// @n Number of iterations to go through
  /// @f Must be invocable with arguments `value_type`, `std::size_t`
  /// @return An iterator one past the last one visited
  template<class InputIt, class Size, class F>
    auto indexed_for_n(InputIt&& first, Size&& n, F&& f) ->
    std::enable_if_t<
      is_iterator_v<InputIt, typename std::iterator_traits<InputIt>::value_type,
        std::input_iterator_tag>
      && std::is_invocable_v<F,
        const typename std::iterator_traits<InputIt>::reference>, InputIt> {
    for (Size i = 0; i < n; ++first, (void) ++i) {
      std::invoke(std::forward<F>(f), *first, i);
    }
    return first;
  }

  /** cldoc:end-category() */
}