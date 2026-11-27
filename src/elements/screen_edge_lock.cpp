#include "adm/elements/screen_edge_lock.hpp"

namespace adm {

  // ---- Getter ---- //
  HorizontalEdge ScreenEdgeLock::get(
      detail::ParameterTraits<HorizontalEdge>::tag) const {
    return horizontal_.value();
  }

  VerticalEdge ScreenEdgeLock::get(
      detail::ParameterTraits<VerticalEdge>::tag) const {
    return vertical_.value();
  }

  // ---- Has ---- //
  bool ScreenEdgeLock::has(detail::ParameterTraits<HorizontalEdge>::tag) const {
    return horizontal_ != std::nullopt;
  }
  bool ScreenEdgeLock::has(detail::ParameterTraits<VerticalEdge>::tag) const {
    return vertical_ != std::nullopt;
  }

  // ---- Setter ---- //
  void ScreenEdgeLock::set(ScreenEdge screenEdge) {
    if (screenEdge == "left" || screenEdge == "right") {
      horizontal_ = HorizontalEdge(screenEdge.get());
    }
    if (screenEdge == "bottom" || screenEdge == "top") {
      vertical_ = VerticalEdge(screenEdge.get());
    }
  }
  void ScreenEdgeLock::set(HorizontalEdge horizontalEdge) {
    horizontal_ = horizontalEdge;
  }
  void ScreenEdgeLock::set(VerticalEdge verticalEdge) {
    vertical_ = verticalEdge;
  }

  // ---- Unsetter ---- //
  void ScreenEdgeLock::unset(detail::ParameterTraits<HorizontalEdge>::tag) {
    horizontal_ = std::nullopt;
  }
  void ScreenEdgeLock::unset(detail::ParameterTraits<VerticalEdge>::tag) {
    vertical_ = std::nullopt;
  }

}  // namespace adm
