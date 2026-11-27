#include "adm/elements/position.hpp"

namespace adm {

  // ---- SPHERICAL POSITION ---- //

  // ---- Constructor ---- //
  SphericalPosition::SphericalPosition(Azimuth azimuth, Elevation elevation)
      : azimuth_(azimuth), elevation_(elevation) {}

  // ---- Defaults ---- //
  const Distance SphericalPosition::distanceDefault_ = Distance(1.f);

  // ---- Getter ---- //
  Azimuth SphericalPosition::get(detail::ParameterTraits<Azimuth>::tag) const {
    return azimuth_;
  }
  Elevation SphericalPosition::get(
      detail::ParameterTraits<Elevation>::tag) const {
    return elevation_;
  }
  Distance SphericalPosition::get(
      detail::ParameterTraits<Distance>::tag) const {
    return distance_.value_or(distanceDefault_);
  }
  ScreenEdgeLock SphericalPosition::get(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_.value();
  }

  // ---- Has ---- //
  bool SphericalPosition::has(detail::ParameterTraits<Azimuth>::tag) const {
    return true;
  }
  bool SphericalPosition::has(detail::ParameterTraits<Elevation>::tag) const {
    return true;
  }
  bool SphericalPosition::has(detail::ParameterTraits<Distance>::tag) const {
    return true;
  }
  bool SphericalPosition::has(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_ != std::nullopt;
  }

  // ---- isDefault ---- //
  bool SphericalPosition::isDefault(
      detail::ParameterTraits<Distance>::tag) const {
    return distance_ == std::nullopt;
  }

  // ---- Setter ---- //
  void SphericalPosition::set(Azimuth azimuth) { azimuth_ = azimuth; }
  void SphericalPosition::set(Elevation elevation) { elevation_ = elevation; }
  void SphericalPosition::set(Distance distance) { distance_ = distance; }
  void SphericalPosition::set(ScreenEdgeLock screenEdgeLock) {
    screenEdgeLock_ = screenEdgeLock;
  }

  // ---- Unsetter ---- //
  void SphericalPosition::unset(detail::ParameterTraits<Distance>::tag) {
    distance_ = std::nullopt;
  }
  void SphericalPosition::unset(detail::ParameterTraits<ScreenEdgeLock>::tag) {
    screenEdgeLock_ = std::nullopt;
  }

  // ---- CARTESIAN POSITION ---- //

  // ---- Constructor ---- //
  CartesianPosition::CartesianPosition(X x, Y y) : x_(x), y_(y) {}

  // ---- Defaults ---- //
  const Z CartesianPosition::zDefault_ = Z(0.f);

  // ---- Getter ---- //
  X CartesianPosition::get(detail::ParameterTraits<X>::tag) const { return x_; }
  Y CartesianPosition::get(detail::ParameterTraits<Y>::tag) const { return y_; }
  Z CartesianPosition::get(detail::ParameterTraits<Z>::tag) const {
    return z_.value_or(zDefault_);
  }
  ScreenEdgeLock CartesianPosition::get(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_.value();
  }

  // ---- Has ---- //
  bool CartesianPosition::has(detail::ParameterTraits<X>::tag) const {
    return true;
  }
  bool CartesianPosition::has(detail::ParameterTraits<Y>::tag) const {
    return true;
  }
  bool CartesianPosition::has(detail::ParameterTraits<Z>::tag) const {
    return true;
  }
  bool CartesianPosition::has(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_ != std::nullopt;
  }

  // ---- isDefault ---- //
  bool CartesianPosition::isDefault(detail::ParameterTraits<Z>::tag) const {
    return z_ == std::nullopt;
  }

  // ---- Setter ---- //
  void CartesianPosition::set(X x) { x_ = x; }
  void CartesianPosition::set(Y y) { y_ = y; }
  void CartesianPosition::set(Z z) { z_ = z; }
  void CartesianPosition::set(ScreenEdgeLock screenEdgeLock) {
    screenEdgeLock_ = screenEdgeLock;
  }

  // ---- Unsetter ---- //
  void CartesianPosition::unset(detail::ParameterTraits<Z>::tag) {
    z_ = std::nullopt;
  }
  void CartesianPosition::unset(detail::ParameterTraits<ScreenEdgeLock>::tag) {
    screenEdgeLock_ = std::nullopt;
  }

  // ---- FREE FUNCTIONS ---- //

  bool isSpherical(const Position& position) {
    if (position.index() == 0) {
      return true;
    } else {
      return false;
    }
  }

  bool isCartesian(const Position& position) {
    if (position.index() == 1) {
      return true;
    } else {
      return false;
    }
  }

}  // namespace adm
