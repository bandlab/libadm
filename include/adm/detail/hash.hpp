#pragma once
#include <functional>

namespace adm::detail {
  template <typename T>
  void hashCombine(std::size_t& seed, const T& value) {
    seed ^= std::hash<T>{}(value) + 0x9e3779b9u + (seed << 6) + (seed >> 2);
  }
}  // namespace adm::detail
