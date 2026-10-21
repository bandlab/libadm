#include "adm/elements/gain_interaction_range.hpp"

namespace adm {

  // ---- Getter ---- //
  GainInteractionMin GainInteractionRange::get(
      detail::ParameterTraits<GainInteractionMin>::tag) const {
    return gainMin_.value();
  }
  GainInteractionMax GainInteractionRange::get(
      detail::ParameterTraits<GainInteractionMax>::tag) const {
    return gainMax_.value();
  }

  // ---- Has ---- //
  bool GainInteractionRange::has(
      detail::ParameterTraits<GainInteractionMin>::tag) const {
    return gainMin_ != std::nullopt;
  }
  bool GainInteractionRange::has(
      detail::ParameterTraits<GainInteractionMax>::tag) const {
    return gainMax_ != std::nullopt;
  }

  // ---- isDefault ---- //
  bool GainInteractionRange::isDefault(
      detail::ParameterTraits<GainInteractionMin>::tag) const {
    return gainMin_ == std::nullopt;
  }
  bool GainInteractionRange::isDefault(
      detail::ParameterTraits<GainInteractionMax>::tag) const {
    return gainMax_ == std::nullopt;
  }

  // ---- Setter ---- //
  void GainInteractionRange::set(GainInteractionMin gainMin) {
    gainMin_ = gainMin;
  }
  void GainInteractionRange::set(GainInteractionMax gainMax) {
    gainMax_ = gainMax;
  }

  // ---- Unsetter ---- //
  void GainInteractionRange::unset(
      detail::ParameterTraits<GainInteractionMin>::tag) {
    gainMin_ = std::nullopt;
  }
  void GainInteractionRange::unset(
      detail::ParameterTraits<GainInteractionMax>::tag) {
    gainMax_ = std::nullopt;
  }

}  // namespace adm
