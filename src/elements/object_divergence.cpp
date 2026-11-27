#include "adm/elements/object_divergence.hpp"

namespace adm {

  // ---- Defaults ---- //
  const Divergence ObjectDivergence::divergenceDefault_ = Divergence(0.f);
  const AzimuthRange ObjectDivergence::azimuthRangeDefault_ =
      AzimuthRange(45.f);
  const PositionRange ObjectDivergence::positionRangeDefault_ =
      PositionRange(0.f);

  // ---- Getter ---- //
  Divergence ObjectDivergence::get(
      detail::ParameterTraits<Divergence>::tag) const {
    return divergence_.value_or(divergenceDefault_);
  }
  AzimuthRange ObjectDivergence::get(
      detail::ParameterTraits<AzimuthRange>::tag) const {
    return azimuthRange_.value_or(azimuthRangeDefault_);
  }
  PositionRange ObjectDivergence::get(
      detail::ParameterTraits<PositionRange>::tag) const {
    return positionRange_.value_or(positionRangeDefault_);
  }

  // ---- Has ---- //
  bool ObjectDivergence::has(detail::ParameterTraits<Divergence>::tag) const {
    return true;
  }
  bool ObjectDivergence::has(detail::ParameterTraits<AzimuthRange>::tag) const {
    return true;
  }
  bool ObjectDivergence::has(
      detail::ParameterTraits<PositionRange>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool ObjectDivergence::isDefault(
      detail::ParameterTraits<Divergence>::tag) const {
    return divergence_ == std::nullopt;
  }
  bool ObjectDivergence::isDefault(
      detail::ParameterTraits<AzimuthRange>::tag) const {
    return azimuthRange_ == std::nullopt;
  }
  bool ObjectDivergence::isDefault(
      detail::ParameterTraits<PositionRange>::tag) const {
    return positionRange_ == std::nullopt;
  }

  // ---- Setter ---- //
  void ObjectDivergence::set(Divergence divergence) {
    divergence_ = divergence;
  }
  void ObjectDivergence::set(AzimuthRange azimuthRange) {
    azimuthRange_ = azimuthRange;
  }
  void ObjectDivergence::set(PositionRange positionRange) {
    positionRange_ = positionRange;
  }

  // ---- Unsetter ---- //
  void ObjectDivergence::unset(detail::ParameterTraits<Divergence>::tag) {
    divergence_ = std::nullopt;
  }
  void ObjectDivergence::unset(detail::ParameterTraits<AzimuthRange>::tag) {
    azimuthRange_ = std::nullopt;
  }
  void ObjectDivergence::unset(detail::ParameterTraits<PositionRange>::tag) {
    positionRange_ = std::nullopt;
  }

}  // namespace adm
