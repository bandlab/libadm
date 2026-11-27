#include "adm/elements/audio_block_format_objects.hpp"

namespace adm {

  // ---- Defaults ---- //
  namespace {
    const ChannelLock channelLockDefault{};
    const ObjectDivergence objectDivergenceDefault{};
    const JumpPosition jumpPositionDefault{};
  }  // namespace

  namespace detail {
    template class RequiredParameter<AudioBlockFormatId>;
    template class DefaultParameter<Width>;
    template class DefaultParameter<Height>;
    template class DefaultParameter<Depth>;
    template class DefaultParameter<Diffuse>;
  }  // namespace detail

  // ---- Getter ---- //
  Cartesian AudioBlockFormatObjects::get(
      detail::ParameterTraits<Cartesian>::tag) const {
    if (cartesian_ != std::nullopt) {
      return cartesian_.value();
    } else {
      if (has<SphericalPosition>()) {
        return Cartesian(false);
      } else {
        return Cartesian(true);
      }
    }
  }
  Position AudioBlockFormatObjects::get(
      detail::ParameterTraits<detail::ParameterTraits<Position>>::tag) const {
    if (has<SphericalPosition>()) {
      return Position(sphericalPosition_.value());
    } else {
      return Position(cartesianPosition_.value());
    }
  }
  SphericalPosition AudioBlockFormatObjects::get(
      detail::ParameterTraits<SphericalPosition>::tag) const {
    return sphericalPosition_.value();
  }
  CartesianPosition AudioBlockFormatObjects::get(
      detail::ParameterTraits<CartesianPosition>::tag) const {
    return cartesianPosition_.value();
  }
  ScreenEdgeLock AudioBlockFormatObjects::get(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_.value();
  }
  ChannelLock AudioBlockFormatObjects::get(
      detail::ParameterTraits<ChannelLock>::tag) const {
    return channelLock_.value_or(channelLockDefault);
  }
  ObjectDivergence AudioBlockFormatObjects::get(
      detail::ParameterTraits<ObjectDivergence>::tag) const {
    return objectDivergence_.value_or(objectDivergenceDefault);
  }
  JumpPosition AudioBlockFormatObjects::get(
      detail::ParameterTraits<JumpPosition>::tag) const {
    return jumpPosition_.value_or(jumpPositionDefault);
  }

  // ---- Has ---- //
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<Cartesian>::tag) const {
    return true;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<detail::ParameterTraits<Position>>::tag) const {
    return sphericalPosition_ != std::nullopt ||
           cartesianPosition_ != std::nullopt;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<SphericalPosition>::tag) const {
    return sphericalPosition_ != std::nullopt;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<CartesianPosition>::tag) const {
    return cartesianPosition_ != std::nullopt;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_ != std::nullopt;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<ChannelLock>::tag) const {
    return true;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<ObjectDivergence>::tag) const {
    return true;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<JumpPosition>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<Cartesian>::tag) const {
    return cartesian_ == std::nullopt;
  }
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<ChannelLock>::tag) const {
    return channelLock_ == std::nullopt;
  }
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<ObjectDivergence>::tag) const {
    return objectDivergence_ == std::nullopt;
  }
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<JumpPosition>::tag) const {
    return jumpPosition_ == std::nullopt;
  }

  // ---- Setter ---- //
  void AudioBlockFormatObjects::set(Cartesian cartesian) {
    cartesian_ = cartesian;

    if (cartesian.get()) {
      sphericalPosition_ = std::nullopt;
      if (!cartesianPosition_) cartesianPosition_ = CartesianPosition();
    } else {
      cartesianPosition_ = std::nullopt;
      if (!sphericalPosition_) sphericalPosition_ = SphericalPosition();
    }
  }
  void AudioBlockFormatObjects::set(Position position) {
    if (position.index() == 0) {
      set(std::get<SphericalPosition>(position));
    } else if (position.index() == 1) {
      set(std::get<CartesianPosition>(position));
    }
  }
  void AudioBlockFormatObjects::set(SphericalPosition position) {
    sphericalPosition_ = position;
    cartesianPosition_ = std::nullopt;
    if (cartesian_ != std::nullopt) {
      cartesian_ = Cartesian(false);
    }
  }
  void AudioBlockFormatObjects::set(CartesianPosition position) {
    cartesianPosition_ = position;
    sphericalPosition_ = std::nullopt;
    cartesian_ = Cartesian(true);
  }
  void AudioBlockFormatObjects::set(ScreenEdgeLock screenEdgeLock) {
    screenEdgeLock_ = screenEdgeLock;
  }
  void AudioBlockFormatObjects::set(ChannelLock channelLock) {
    channelLock_ = channelLock;
  }
  void AudioBlockFormatObjects::set(ObjectDivergence objectDivergence) {
    objectDivergence_ = objectDivergence;
  }
  void AudioBlockFormatObjects::set(JumpPosition jumpPosition) {
    jumpPosition_ = jumpPosition;
  }

  // ---- Unsetter ---- //
  void AudioBlockFormatObjects::unset(detail::ParameterTraits<Cartesian>::tag) {
    cartesian_ = std::nullopt;
    if (!has<SphericalPosition>()) {
      set(SphericalPosition{});
    }
  }
  void AudioBlockFormatObjects::unset(
      detail::ParameterTraits<detail::ParameterTraits<Position>>::tag) {
    sphericalPosition_ = std::nullopt;
    cartesianPosition_ = std::nullopt;
  }
  void AudioBlockFormatObjects::unset(
      detail::ParameterTraits<SphericalPosition>::tag) {
    sphericalPosition_ = std::nullopt;
  }
  void AudioBlockFormatObjects::unset(
      detail::ParameterTraits<CartesianPosition>::tag) {
    cartesianPosition_ = std::nullopt;
  }
  void AudioBlockFormatObjects::unset(
      detail::ParameterTraits<ScreenEdgeLock>::tag) {
    screenEdgeLock_ = std::nullopt;
  }
  void AudioBlockFormatObjects::unset(
      detail::ParameterTraits<ChannelLock>::tag) {
    channelLock_ = std::nullopt;
  }
  void AudioBlockFormatObjects::unset(
      detail::ParameterTraits<ObjectDivergence>::tag) {
    objectDivergence_ = std::nullopt;
  }
  void AudioBlockFormatObjects::unset(
      detail::ParameterTraits<JumpPosition>::tag) {
    jumpPosition_ = std::nullopt;
  }

}  // namespace adm
