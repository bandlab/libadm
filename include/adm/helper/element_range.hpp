#pragma once

#include <algorithm>
#include <iterator>
#include <memory>
#include <ranges>
#include <vector>

namespace adm {

  namespace detail {

    /**
     * Converts shared or weak pointers to pointers to const elements.
     * @headerfile element_range.hpp <adm/helpers/element_range.hpp>
     */
    template <typename Pointer>
    struct ConstPointer {
      const Pointer operator()(const auto& pointer) const { return pointer; }
    };

    /**
     * Helper template to get the range type for
     * `std::shared_ptr<Element>` in an `std::vector`.
     * @sa struct ElementRangeImpl<const Element>
     * @headerfile element_range.hpp <adm/helpers/element_range.hpp>
     */
    template <typename Element>
    struct ElementRangeImpl {
      using type = std::ranges::subrange<
          typename std::vector<std::shared_ptr<Element>>::const_iterator>;
    };

    /**
     * Specialization of the helper template to get the range type
     * for `std::shared_ptr<const Element>` in a `std::vector`.
     *
     * A transform view converts pointers to const elements on dereference.
     * @headerfile element_range.hpp <adm/helpers/element_range.hpp>
     */
    template <typename Element>
    struct ElementRangeImpl<const Element> {
      using type = std::ranges::transform_view<
          typename ElementRangeImpl<Element>::type,
          ConstPointer<std::shared_ptr<const Element>>>;
    };

    /**
     * Helper template to get the range type for
     * `std::weak_ptr<Element>` in an `std::vector`.
     * @sa struct ElementRangeImpl<const Element>
     * @headerfile element_range.hpp <adm/helpers/element_range.hpp>
     */
    template <typename Element>
    struct ElementRangeWeakImpl {
      using type = std::ranges::subrange<
          typename std::vector<std::weak_ptr<Element>>::const_iterator>;
    };

    /**
     * Specialization of the helper template to get the range type
     * for `std::weak_ptr<const Element>` in a `std::vector`.
     *
     * A transform view converts pointers to const elements on dereference.
     * @headerfile element_range.hpp <adm/helpers/element_range.hpp>
     */
    template <typename Element>
    struct ElementRangeWeakImpl<const Element> {
      using type = std::ranges::transform_view<
          typename ElementRangeWeakImpl<Element>::type,
          ConstPointer<std::weak_ptr<const Element>>>;
    };

  }  // namespace detail

  /**
   * @brief Proxy access to a STL iterator range of Adm elements
   * @headerfile element_range.hpp <adm/helpers/element_range.hpp>
   *
   * This templated alias gives access to a range of `std::shared_ptr<>`
   * to Adm elements.
   *
   * This value type of the underlying iterator/range will be either
   * `std::shared_ptr<Element>` or `const std::shared_ptr<const      *
   * Element`> in the case of `const Element` template parameter.
   *
   * The latter case is the main reason for the existence of this alias and
   * its supporting classes, as this allows as to return a  range of "real"
   * `const` elements.
   *
   * The range uses `std::ranges::subrange`, with a `std::ranges::transform_view`
   * for const elements.
   *
   * One make use the convenience `detail::makeElementRange()` function to
   * quickly create a range from an container.
   *
   * @code
    ElementRange<AudioObject> getMutableAudioObjects();
    ElementRange<const AudioObject> getConstAudioObjects();

    // ...

    for (const auto element& : getMutableAudioObjects()) {
      // element is of type std::shared_ptr<AudioObject>
      // ...
     }

    for (const auto element& : getMutableAudioObjects()) {
       // element is of type std::shared_ptr<const AudioObject>
       // ...
    }
   @endcode
   *
   * @note
   * Please note that the range will *always* be represented by
   * `const_iterator`s. The difference is between `Element` and `const
   * Element` is the *value type* of the iterators.
   *
   * The rationale for this is that it's not allowed to change the range
   * itself via its iterators, because there are member functions of the
   * parent elements to achieve this and ensure internal invariants to hold.
   *
   * Iterators for const elements must not outlive the range view. This also
   * applies to ElementWeakRange<const Element>.
   */
  template <typename Element>
  using ElementRange = typename detail::ElementRangeImpl<Element>::type;

  template <typename Element>
  using ElementWeakRange = typename detail::ElementRangeWeakImpl<Element>::type;

  namespace detail {

    /**
     * Convenience function overload to create an range to **const**
     * `Element`s when given a **const** Container.
     * @sa makeElementRange(Container&);
     */
    template <typename Element, typename Container>
    ElementRange<const Element> makeElementRange(const Container &container) {
      return {std::ranges::subrange(container.cbegin(), container.cend()),
              ConstPointer<std::shared_ptr<const Element>>{}};
    }

    /**
     * Convenience function overload to create an range to
     * `Element`s when given a **non-const** Container.
     * @sa makeElementRange(const Container&);
     */
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
