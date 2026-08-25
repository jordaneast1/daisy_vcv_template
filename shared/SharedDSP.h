#pragma once

#include "daisysp.h"

namespace shared
{
/** The voice shared by the Daisy firmware and the VCV Rack module.
 *
 *  This holds all of the DSP and none of the front panel. Each host reads its
 *  own knob and button however it likes, then hands the values to Process(),
 *  so both sides always sound the same.
 */
class SharedDSP
{
  public:
    /** Call before Process(), and again whenever the sample rate changes. */
    void Init(float sample_rate);

    /** Advance the voice by one sample.
     *
     *  pitch_knob  0..1 knob position, mapped to MIDI note 0..127
     *  gate        true while the envelope should rise and hold at sustain,
     *              false while it releases
     *
     *  Returns the oscillator output, nominally -1..1.
     */
    float Process(float pitch_knob, bool gate);

    /** Envelope level from the most recent Process() call, 0..1. */
    float GetEnvelopeLevel() const { return env_level_; }

  private:
    daisysp::Oscillator osc_;
    daisysp::Adsr       env_;
    float               env_level_ = 0.f;
};

} // namespace shared
