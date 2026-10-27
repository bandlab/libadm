#include <catch2/catch.hpp>
#include <algorithm>
#include <ranges>
#include <type_traits>
#include <utility>
#include "adm/helper/element_range.hpp"
#include "adm/elements/audio_channel_format.hpp"

namespace {
  using MutableRange = adm::ElementRange<int>;
  using ConstRange = adm::ElementRange<const int>;
  using WeakRange = adm::ElementWeakRange<const int>;

  static_assert(std::ranges::random_access_range<MutableRange>);
  static_assert(std::ranges::forward_range<const ConstRange>);
  static_assert(std::ranges::forward_range<const WeakRange>);
  static_assert(std::is_same_v<decltype(*std::declval<MutableRange>().begin()),
                               const std::shared_ptr<int>&>);
  static_assert(std::is_same_v<decltype(*std::declval<ConstRange>().begin()),
                               const std::shared_ptr<const int>>);
  static_assert(std::is_same_v<decltype(*std::declval<WeakRange>().begin()),
                               const std::weak_ptr<const int>>);
  static_assert(
      std::is_same_v<decltype(std::declval<adm::BlockFormatsRange<int>>()[0]),
                     int&>);
  static_assert(std::is_same_v<
                decltype(std::declval<adm::BlockFormatsConstRange<int>>()[0]),
                const int&>);
}  // namespace

TEST_CASE("element_ranges_borrow_pointers_and_preserve_constness") {
  std::vector<std::shared_ptr<int>> elements{std::make_shared<int>(1), nullptr,
                                             std::make_shared<int>(3)};
  auto mutableRange = adm::detail::makeElementRange<int>(elements);
  const auto range =
      adm::detail::makeElementRange<int>(std::as_const(elements));

  REQUIRE(range.size() == 3);
  REQUIRE_FALSE(range.empty());
  REQUIRE(range.front() == elements.front());
  REQUIRE(range.back() == elements.back());
  REQUIRE(range[1] == nullptr);
  REQUIRE(std::ranges::equal(range, elements));
  REQUIRE(elements.front().use_count() == 1);

  *mutableRange.front() = 7;
  REQUIRE(*range.front() == 7);

  // Replacement without vector reallocation is visible through both ranges.
  elements[1] = std::make_shared<int>(2);
  REQUIRE(*range[1] == 2);
  REQUIRE(mutableRange[1] == elements[1]);

  auto it = range.begin();
  REQUIRE(it++ == range.begin());
  REQUIRE(*it == elements[1]);
  auto copy = it;
  REQUIRE(*++it == elements[2]);
  REQUIRE(*copy == elements[1]);
  REQUIRE(std::next(copy) == it);
  REQUIRE(std::distance(range.begin(), range.end()) == 3);
  REQUIRE(std::find(range.begin(), range.end(), elements[2]) == it);
  REQUIRE(++it == range.end());

  // Iterators refer to the container, not to a temporary range object.
  auto borrowed =
      adm::detail::makeElementRange<int>(std::as_const(elements)).begin();
  REQUIRE(*borrowed == elements[0]);
}

TEST_CASE("element_ranges_allow_empty_containers") {
  const std::vector<std::shared_ptr<int>> elements;
  const auto range = adm::detail::makeElementRange<int>(elements);
  REQUIRE(range.empty());
  REQUIRE(range.size() == 0);
  REQUIRE(range.begin() == range.end());
  REQUIRE(std::distance(range.begin(), range.end()) == 0);
  REQUIRE(std::ranges::equal(range, elements));
  REQUIRE(ConstRange{}.begin() == ConstRange{}.end());
  REQUIRE(WeakRange{}.empty());
  REQUIRE(MutableRange{}.empty());
}

TEST_CASE("const_weak_element_ranges_do_not_retain_elements") {
  auto element = std::make_shared<int>(42);
  std::vector<std::weak_ptr<int>> elements{element, {}};
  const WeakRange range{elements};
  REQUIRE(range.size() == 2);
  REQUIRE(range.front().lock() == element);
  REQUIRE(range.back().expired());
  REQUIRE((*range.begin()).lock() == element);
  REQUIRE(element.use_count() == 1);
  element.reset();
  REQUIRE(range[0].expired());
  REQUIRE((*range.begin()).expired());
}
