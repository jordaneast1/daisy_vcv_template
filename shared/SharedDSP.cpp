#include "SharedDSP.h"

using namespace daisysp;

namespace
{
// Envelope shape. Kept in one place so the Daisy build and the VCV build
// can't drift apart.
const float kAttack  = .01f;
const float kDecay   = .4f;
const float kRelease = .4f;
const float kSustain = 1.f;
} // namespace

namespace shared
{
void SharedDSP::Init(float sample_rate)
{
    osc_.Init(sample_rate);
    osc_.SetWaveform(Oscillator::WAVE_SAW);

    env_.Init(sample_rate);
    env_.SetTime(ADSR_SEG_ATTACK, kAttack);
    env_.SetTime(ADSR_SEG_DECAY, kDecay);
    env_.SetTime(ADSR_SEG_RELEASE, kRelease);
    // Hold at full level for as long as the gate is held
    env_.SetSustainLevel(kSustain);

    env_level_ = 0.f;
}

float SharedDSP::Process(float pitch_knob, bool gate)
{
    // Convert the knob position to midi (0-127), then midi to freq. in Hz
    osc_.SetFreq(mtof(pitch_knob * 127.f));

    // While the gate is high the envelope rises and holds at sustain,
    // when it goes low the envelope releases
    env_level_ = env_.Process(gate);
    // Set the oscillator volume to the latest env value
    osc_.SetAmp(env_level_);

    return osc_.Process();
}

} // namespace shared
