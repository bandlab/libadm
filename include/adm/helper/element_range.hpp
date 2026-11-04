#pragma once

#include <algorithm>
#include <iterator>
#include <memory>
#include <ranges>
#include <vector>

namespace adm {

  namespace detail {

    /// Converts a pointer to a pointer to const.
    template <typename Pointer>
    struct ConstPointer {
      const Pointer operator()(const auto& pointer) const { return pointer; }
    };

    /// Range over `std::shared_ptr<Element>`.
    template <typename Element>
    struct ElementRangeImpl {
      using type = std::ranges::subrange<
          typename std::vector<std::shared_ptr<Element>>::const_iterator>;
    };

    /// Range over `std::shared_ptr<const Element>`.
    template <typename Element>
    struct ElementRangeImpl<const Element> {
      using type = std::ranges::transform_view<
          typename ElementRangeImpl<Element>::type,
          ConstPointer<std::shared_ptr<const Element>>>;
    };

    /// Range over `std::weak_ptr<Element>`.
    template <typename Element>
    struct ElementRangeWeakImpl {
      using type = std::ranges::subrange<
          typename std::vector<std::weak_ptr<Element>>::const_iterator>;
    };

    /// Range over `std::weak_ptr<const Element>`.
    template <typename Element>
    struct ElementRangeWeakImpl<const Element> {
      using type = std::ranges::transform_view<
          typename ElementRangeWeakImpl<Element>::type,
          ConstPointer<std::weak_ptr<const Element>>>;
    };

  }  // namespace detail

  /// Read-only range of ADM element pointers. Const element iterators must
  /// not outlive the range.
  template <typename Element>
  using ElementRange = typename detail::ElementRangeImpl<Element>::type;

  template <typename Element>
  using ElementWeakRange = typename detail::ElementRangeWeakImpl<Element>::type;

  namespace detail {

    /// Creates a range to const elements.
    template <typename Element, typename Container>
    ElementRange<const Element> makeElementRange(const Container &container) {
      return {std::ranges::subrange(container.cbegin(), container.cend()),
              ConstPointer<std::shared_ptr<const Element>>{}};
    }

    /// Creates a range to mutable elements.
    template <typename Element, typename Container>
    ElementRange<Element> makeElementRange(Container &container) {
      return {container.cbegin(), container.cend()};
    }

    template <typename Element, typename Container>
    std::vector<std::shared_ptr<Element>> lockWeakRange(const Container &c) {
      std::vector<std::shared_ptr<Element>> result;
      std::transform(c.cbegin(), c.cend(), std::back_inserter(result),
                     [](std::weak_ptr<Element> w) { return w.lock(); });
      return result;
    }

  }  // namespace detail

}  // namespace adm
