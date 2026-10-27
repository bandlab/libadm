#include "adm/elements/speaker_position.hpp"

namespace adm {
  // ---- Defaults ---- //
  namespace {
    // N.B. these defaults are not currently specified in 2076 for
    // directspeaker block formats, but these sub-elements are optional,
    // so some default is required for rendering. These defaults match
    // those used in audioobject type audioBlockFormats
    const Distance defaultDistance = Distance{1.f};
    const Z defaultZ = Z{0.f};
  }  // namespace

  // ---- Getter ---- //
  X CartesianSpeakerPosition::get(detail::ParameterTraits<X>::tag) const {
    return x_;
  }
  XMin CartesianSpeakerPosition::get(detail::ParameterTraits<XMin>::tag) const {
    return xMin_.value();
  }
  XMax CartesianSpeakerPosition::get(detail::ParameterTraits<XMax>::tag) const {
    return xMax_.value();
  }
  Y CartesianSpeakerPosition::get(detail::ParameterTraits<Y>::tag) const {
    return y_;
  }
  YMin CartesianSpeakerPosition::get(detail::ParameterTraits<YMin>::tag) const {
    return yMin_.value();
  }
  YMax CartesianSpeakerPosition::get(detail::ParameterTraits<YMax>::tag) const {
    return yMax_.value();
  }
  Z CartesianSpeakerPosition::get(detail::ParameterTraits<Z>::tag) const {
    return z_.value_or(defaultZ);
  }
  ZMin CartesianSpeakerPosition::get(detail::ParameterTraits<ZMin>::tag) const {
    return zMin_.value();
  }
  ZMax CartesianSpeakerPosition::get(detail::ParameterTraits<ZMax>::tag) const {
    return zMax_.value();
  }
  ScreenEdgeLock CartesianSpeakerPosition::get(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_.value();
  }

  // ---- Has ---- //
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<X>::tag) const {
    return true;
  }
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<XMin>::tag) const {
    return xMin_ != std::nullopt;
  }
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<XMax>::tag) const {
    return xMax_ != std::nullopt;
  }
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<Y>::tag) const {
    return true;
  }
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<YMin>::tag) const {
    return yMin_ != std::nullopt;
  }
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<YMax>::tag) const {
    return yMax_ != std::nullopt;
  }
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<Z>::tag) const {
    return z_ != std::nullopt;
  }
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<ZMin>::tag) const {
    return zMin_ != std::nullopt;
  }
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<ZMax>::tag) const {
    return zMax_ != std::nullopt;
  }
  bool CartesianSpeakerPosition::has(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_ != std::nullopt;
  }

  bool CartesianSpeakerPosition::isDefault(
      detail::ParameterTraits<Z>::tag) const {
    return !has<Z>();
  }

  // ---- Setter ---- //
  void CartesianSpeakerPosition::set(X x) { x_ = x; }
  void CartesianSpeakerPosition::set(XMin xMin) { xMin_ = xMin; }
  void CartesianSpeakerPosition::set(XMax xMax) { xMax_ = xMax; }
  void CartesianSpeakerPosition::set(Y y) { y_ = y; }
  void CartesianSpeakerPosition::set(YMin yMin) { yMin_ = yMin; }
  void CartesianSpeakerPosition::set(YMax yMax) { yMax_ = yMax; }
  void CartesianSpeakerPosition::set(Z z) { z_ = z; }
  void CartesianSpeakerPosition::set(ZMin zMin) { zMin_ = zMin; }
  void CartesianSpeakerPosition::set(ZMax zMax) { zMax_ = zMax; }
  void CartesianSpeakerPosition::set(ScreenEdgeLock screenEdgeLock) {
    screenEdgeLock_ = ScreenEdgeLock(screenEdgeLock);
  }

  // ---- Unsetter ---- //
  void CartesianSpeakerPosition::unset(detail::ParameterTraits<XMin>::tag) {
    xMin_ = std::nullopt;
  }
  void CartesianSpeakerPosition::unset(detail::ParameterTraits<XMax>::tag) {
    xMax_ = std::nullopt;
  }
  void CartesianSpeakerPosition::unset(detail::ParameterTraits<YMin>::tag) {
    yMin_ = std::nullopt;
  }
  void CartesianSpeakerPosition::unset(detail::ParameterTraits<YMax>::tag) {
    yMax_ = std::nullopt;
  }
  void CartesianSpeakerPosition::unset(detail::ParameterTraits<Z>::tag) {
    z_ = std::nullopt;
  }
  void CartesianSpeakerPosition::unset(detail::ParameterTraits<ZMin>::tag) {
    zMin_ = std::nullopt;
  }
  void CartesianSpeakerPosition::unset(detail::ParameterTraits<ZMax>::tag) {
    zMax_ = std::nullopt;
  }
  void CartesianSpeakerPosition::unset(
      detail::ParameterTraits<ScreenEdgeLock>::tag) {
    screenEdgeLock_ = std::nullopt;
  }

  // ---- Getter ---- //
  Azimuth SphericalSpeakerPosition::get(
      detail::ParameterTraits<Azimuth>::tag) const {
    return azimuth_;
  }
  AzimuthMin SphericalSpeakerPosition::get(
      detail::ParameterTraits<AzimuthMin>::tag) const {
    return azimuthMin_.value();
  }
  AzimuthMax SphericalSpeakerPosition::get(
      detail::ParameterTraits<AzimuthMax>::tag) const {
    return azimuthMax_.value();
  }
  Elevation SphericalSpeakerPosition::get(
      detail::ParameterTraits<Elevation>::tag) const {
    return elevation_;
  }
  ElevationMin SphericalSpeakerPosition::get(
      detail::ParameterTraits<ElevationMin>::tag) const {
    return elevationMin_.value();
  }
  ElevationMax SphericalSpeakerPosition::get(
      detail::ParameterTraits<ElevationMax>::tag) const {
    return elevationMax_.value();
  }
  Distance SphericalSpeakerPosition::get(
      detail::ParameterTraits<Distance>::tag) const {
    return distance_.value_or(defaultDistance);
  }
  DistanceMin SphericalSpeakerPosition::get(
      detail::ParameterTraits<DistanceMin>::tag) const {
    return distanceMin_.value();
  }
  DistanceMax SphericalSpeakerPosition::get(
      detail::ParameterTraits<DistanceMax>::tag) const {
    return distanceMax_.value();
  }
  ScreenEdgeLock SphericalSpeakerPosition::get(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_.value();
  }

  // ---- Has ---- //
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<Azimuth>::tag) const {
    return true;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<AzimuthMin>::tag) const {
    return azimuthMin_ != std::nullopt;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<AzimuthMax>::tag) const {
    return azimuthMax_ != std::nullopt;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<Elevation>::tag) const {
    return true;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<ElevationMin>::tag) const {
    return elevationMin_ != std::nullopt;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<ElevationMax>::tag) const {
    return elevationMax_ != std::nullopt;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<Distance>::tag) const {
    return distance_ != std::nullopt;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<DistanceMin>::tag) const {
    return distanceMin_ != std::nullopt;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<DistanceMax>::tag) const {
    return distanceMax_ != std::nullopt;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_ != std::nullopt;
  }

  bool SphericalSpeakerPosition::isDefault(
      detail::ParameterTraits<Distance>::tag) const {
    return !has<Distance>();
  }

  // ---- Setter ---- //
  void SphericalSpeakerPosition::set(Azimuth azimuth) { azimuth_ = azimuth; }
  void SphericalSpeakerPosition::set(AzimuthMin azimuthMin) {
    azimuthMin_ = azimuthMin;
  }
  void SphericalSpeakerPosition::set(AzimuthMax azimuthMax) {
    azimuthMax_ = azimuthMax;
  }
  void SphericalSpeakerPosition::set(Elevation elevation) {
    elevation_ = elevation;
  }
  void SphericalSpeakerPosition::set(ElevationMin elevationMin) {
    elevationMin_ = elevationMin;
  }
  void SphericalSpeakerPosition::set(ElevationMax elevationMax) {
    elevationMax_ = elevationMax;
  }
  void SphericalSpeakerPosition::set(Distance distance) {
    distance_ = distance;
  }
  void SphericalSpeakerPosition::set(DistanceMin distanceMin) {
    distanceMin_ = distanceMin;
  }
  void SphericalSpeakerPosition::set(DistanceMax distanceMax) {
    distanceMax_ = distanceMax;
  }
  void SphericalSpeakerPosition::set(ScreenEdgeLock screenEdgeLock) {
    screenEdgeLock_ = ScreenEdgeLock(screenEdgeLock);
  }

  // ---- Unsetter ---- //
  void SphericalSpeakerPosition::unset(
      detail::ParameterTraits<AzimuthMin>::tag) {
    azimuthMin_ = std::nullopt;
  }
  void SphericalSpeakerPosition::unset(
      detail::ParameterTraits<AzimuthMax>::tag) {
    azimuthMax_ = std::nullopt;
  }
  void SphericalSpeakerPosition::unset(
      detail::ParameterTraits<ElevationMin>::tag) {
    elevationMin_ = std::nullopt;
  }
  void SphericalSpeakerPosition::unset(
      detail::ParameterTraits<ElevationMax>::tag) {
    elevationMax_ = std::nullopt;
  }
  void SphericalSpeakerPosition::unset(detail::ParameterTraits<Distance>::tag) {
    distance_ = std::nullopt;
  }
  void SphericalSpeakerPosition::unset(
      detail::ParameterTraits<DistanceMin>::tag) {
    distanceMin_ = std::nullopt;
  }
  void SphericalSpeakerPosition::unset(
      detail::ParameterTraits<DistanceMax>::tag) {
    distanceMax_ = std::nullopt;
  }
  void SphericalSpeakerPosition::unset(
      detail::ParameterTraits<ScreenEdgeLock>::tag) {
    screenEdgeLock_ = std::nullopt;
  }

}  // namespace adm
