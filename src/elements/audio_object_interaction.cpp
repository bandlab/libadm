#include "adm/elements/audio_object_interaction.hpp"

namespace adm {

  // ---- Getter ---- //
  OnOffInteract AudioObjectInteraction::get(
      detail::ParameterTraits<OnOffInteract>::tag) const {
    return onOffInteract_;
  }
  GainInteract AudioObjectInteraction::get(
      detail::ParameterTraits<GainInteract>::tag) const {
    return gainInteract_.value();
  }
  PositionInteract AudioObjectInteraction::get(
      detail::ParameterTraits<PositionInteract>::tag) const {
    return positionInteract_.value();
  }
  GainInteractionRange AudioObjectInteraction::get(
      detail::ParameterTraits<GainInteractionRange>::tag) const {
    return gainInteractionRange_.value();
  }
  PositionInteractionRange AudioObjectInteraction::get(
      detail::ParameterTraits<PositionInteractionRange>::tag) const {
    return positionInteractionRange_.value();
  }

  // ---- Has ---- //
  bool AudioObjectInteraction::has(
      detail::ParameterTraits<OnOffInteract>::tag) const {
    return true;
  }
  bool AudioObjectInteraction::has(
      detail::ParameterTraits<GainInteract>::tag) const {
    return gainInteract_ != std::nullopt;
  }
  bool AudioObjectInteraction::has(
      detail::ParameterTraits<PositionInteract>::tag) const {
    return positionInteract_ != std::nullopt;
  }
  bool AudioObjectInteraction::has(
      detail::ParameterTraits<GainInteractionRange>::tag) const {
    return gainInteractionRange_ != std::nullopt;
  }
  bool AudioObjectInteraction::has(
      detail::ParameterTraits<PositionInteractionRange>::tag) const {
    return positionInteractionRange_ != std::nullopt;
  }

  // ---- Setter ---- //
  void AudioObjectInteraction::set(OnOffInteract onOffInteract) {
    onOffInteract_ = onOffInteract;
  }
  void AudioObjectInteraction::set(GainInteract gainInteract) {
    gainInteract_ = gainInteract;
  }
  void AudioObjectInteraction::set(PositionInteract positionInteract) {
    positionInteract_ = positionInteract;
  }
  void AudioObjectInteraction::set(GainInteractionRange gainInteractionRange) {
    gainInteractionRange_ = gainInteractionRange;
  }
  void AudioObjectInteraction::set(
      PositionInteractionRange positionInteractionRange) {
    positionInteractionRange_ = positionInteractionRange;
  }

  // ---- Unsetter ---- //
  void AudioObjectInteraction::unset(
      detail::ParameterTraits<GainInteract>::tag) {
    gainInteract_ = std::nullopt;
  }
  void AudioObjectInteraction::unset(
      detail::ParameterTraits<PositionInteract>::tag) {
    positionInteract_ = std::nullopt;
  }
  void AudioObjectInteraction::unset(
      detail::ParameterTraits<GainInteractionRange>::tag) {
    gainInteractionRange_ = std::nullopt;
  }
  void AudioObjectInteraction::unset(
      detail::ParameterTraits<PositionInteractionRange>::tag) {
    positionInteractionRange_ = std::nullopt;
  }

}  // namespace adm
