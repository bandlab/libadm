#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <span>
#include <vector>

namespace adm {

  namespace detail {

    // Converts shared/weak pointers on dereference without copying the container.
    // Clang 14 with libstdc++ 11 cannot instantiate std::ranges::view_interface.
    template <typename Pointer, typename ConstPointer>
    class ConstPointerRange {
      using Storage = std::span<const Pointer>;

     public:
      class iterator {
       public:
        using value_type = ConstPointer;
        using reference = const value_type;
        using pointer = void;
        using difference_type = std::ptrdiff_t;
        using iterator_concept = std::forward_iterator_tag;
        // Legacy forward iterators require an lvalue reference on dereference.
        using iterator_category = std::input_iterator_tag;

        iterator() = default;
        explicit iterator(typename Storage::iterator current)
            : current_(current) {}
        reference operator*() const { return *current_; }
        iterator &operator++() {
          ++current_;
          return *this;
        }
        iterator operator++(int) {
          auto previous = *this;
          ++*this;
          return previous;
        }
        bool operator==(const iterator &) const = default;

       private:
        typename Storage::iterator current_{};
      };

      ConstPointerRange() = default;
      explicit ConstPointerRange(Storage elements) : elements_(elements) {}
      iterator begin() const { return iterator{elements_.begin()}; }
      iterator end() const { return iterator{elements_.end()}; }
      std::size_t size() const { return elements_.size(); }
      bool empty() const { return elements_.empty(); }
      explicit operator bool() const { return !empty(); }
      const ConstPointer operator[](std::size_t index) const {
        return elements_[index];
      }
      const ConstPointer front() const { return elements_.front(); }
      const ConstPointer back() const { return elements_.back(); }

     private:
      Storage elements_;
    };

    /**
     * Helper template to get the range type for
     * `std::shared_ptr<Element>` in an `std::vector`.
     * @sa struct ElementRangeImpl<const Element>
     * @headerfile element_range.hpp <adm/helpers/element_range.hpp>
     */
    template <typename Element>
    struct ElementRangeImpl {
      using type = std::span<const std::shared_ptr<Element>>;
    };

    /**
     * Specialization of the helper template to get the range type
     * for `std::shared_ptr<const Element>` in a `std::vector`.
     *
     * Pointers are converted to const elements on dereference.
     * @headerfile element_range.hpp <adm/helpers/element_range.hpp>
     */
    template <typename Element>
    struct ElementRangeImpl<const Element> {
      using type = ConstPointerRange<std::shared_ptr<Element>,
                                     std::shared_ptr<const Element>>;
    };

    /**
     * Helper template to get the range type for
     * `std::weak_ptr<Element>` in an `std::vector`.
     * @sa struct ElementRangeImpl<const Element>
     * @headerfile element_range.hpp <adm/helpers/element_range.hpp>
     */
    template <typename Element>
    struct ElementRangeWeakImpl {
      using type = std::span<const std::weak_ptr<Element>>;
    };

    /**
     * Specialization of the helper template to get the range type
     * for `std::weak_ptr<const Element>` in a `std::vector`.
     *
     * Pointers are converted to const elements on dereference.
     * @headerfile element_range.hpp <adm/helpers/element_range.hpp>
     */
    template <typename Element>
    struct ElementRangeWeakImpl<const Element> {
      using type = ConstPointerRange<std::weak_ptr<Element>,
                                     std::weak_ptr<const Element>>;
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
   * The range uses `std::span`, with pointer conversion on dereference
   * for const elements. No elements are copied or allocated. Const element
   * iterators support forward traversal; indexed access is provided by the range.
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
   * The stored pointers cannot be modified through the range. The difference
   * between `Element` and `const Element` is the pointee type of the pointers.
   *
   * The rationale for this is that it's not allowed to change the range
   * itself via its iterators, because there are member functions of the
   * parent elements to achieve this and ensure internal invariants to hold.
   *
   * Ranges and their iterators must not outlive the underlying container or
   * a container operation that invalidates its iterators.
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
      return ElementRange<const Element>{container};
    }

    /**
     * Convenience function overload to create an range to
     * `Element`s when given a **non-const** Container.
     * @sa makeElementRange(const Container&);
     */
    template <typename Element, typename Container>
    ElementRange<Element> makeElementRange(Container &container) {
      return ElementRange<Element>{container};
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
