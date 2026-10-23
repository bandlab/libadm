#include "adm/elements/audio_block_format_binaural.hpp"

namespace adm {
  namespace {
    const Rtime rtimeDefault{std::chrono::seconds(0)};
  }

  // ---- Getter ---- //
  AudioBlockFormatId AudioBlockFormatBinaural::get(
      detail::ParameterTraits<AudioBlockFormatId>::tag) const {
    return id_;
  }
  Rtime AudioBlockFormatBinaural::get(
      detail::ParameterTraits<Rtime>::tag) const {
    return rtime_.value_or(rtimeDefault);
  }
  Duration AudioBlockFormatBinaural::get(
      detail::ParameterTraits<Duration>::tag) const {
    return duration_.value();
  }

  // ---- Has ---- //
  bool AudioBlockFormatBinaural::has(
      detail::ParameterTraits<AudioBlockFormatId>::tag) const {
    return true;
  }
  bool AudioBlockFormatBinaural::has(
      detail::ParameterTraits<Rtime>::tag) const {
    return true;
  }
  bool AudioBlockFormatBinaural::has(
      detail::ParameterTraits<Duration>::tag) const {
    return duration_ != std::nullopt;
  }

  // ---- isDefault ---- //
  bool AudioBlockFormatBinaural::isDefault(
      detail::ParameterTraits<Rtime>::tag) const {
    return duration_ == std::nullopt;
  }

  // ---- Setter ---- //
  void AudioBlockFormatBinaural::set(AudioBlockFormatId id) { id_ = id; }
  void AudioBlockFormatBinaural::set(Rtime rtime) { rtime_ = rtime; }
  void AudioBlockFormatBinaural::set(Duration duration) {
    duration_ = duration;
  }

  // ---- Unsetter ---- //
  void AudioBlockFormatBinaural::unset(detail::ParameterTraits<Rtime>::tag) {
    rtime_ = std::nullopt;
  }
  void AudioBlockFormatBinaural::unset(detail::ParameterTraits<Duration>::tag) {
    duration_ = std::nullopt;
  }

}  // namespace adm
