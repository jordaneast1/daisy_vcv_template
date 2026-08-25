#include "daisy_seed.h"
#include "daisysp.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;
using namespace daisysp;

// Declare a DaisySeed object called hardware
DaisySeed  hardware;
Oscillator osc;
Adsr       env;

//Configure and initialize button
Switch button1;

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    float osc_out, env_out;
    bool  gate;

    //Nobody likes a bouncy button
    button1.Debounce();

    //Hold the button to hold the envelope at its sustain level
    gate = button1.Pressed();

    //Light the onboard LED while the button is held down
    hardware.SetLed(gate);

    //Convert floating point knob to midi (0-127)
    //Then convert midi to freq. in Hz
    osc.SetFreq(mtof(hardware.adc.GetFloat(0) * 127));

    //Fill the block with samples
    for(size_t i = 0; i < size; i += 2)
    {
        //Get the next envelope value
        //While the gate is high the envelope rises and holds at sustain,
        //when it goes low the envelope releases
        env_out = env.Process(gate);
        //Set the oscillator volume to the latest env value
        osc.SetAmp(env_out);
        //get the next oscillator sample
        osc_out = osc.Process();

        //Set the left and right outputs
        out[i]     = osc_out;
        out[i + 1] = osc_out;
    }
}


int main(void)
{
    // Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    hardware.Configure();
    hardware.Init();
    hardware.SetAudioBlockSize(4);


    //How many samples we'll output per second
    float samplerate = hardware.AudioSampleRate();

    //Create an ADC configuration
    AdcChannelConfig adcConfig;
    //Add pin 21 as an analog input in this config. We'll use this to read the knob
    adcConfig.InitSingle(hardware.GetPin(21));

    //Initialize the button on pin 28
    button1.Init(hardware.GetPin(28), samplerate / 48.f);

    //Set the ADC to use our configuration
    hardware.adc.Init(&adcConfig, 1);

    //Set up oscillator
    osc.Init(samplerate);
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetAmp(2.f);
    osc.SetFreq(1000);

    //Set up volume envelope
    env.Init(samplerate);
    //Envelope segment times
    env.SetTime(ADSR_SEG_ATTACK, .01f);
    env.SetTime(ADSR_SEG_DECAY, .4f);
    env.SetTime(ADSR_SEG_RELEASE, .4f);
    //Hold at full level for as long as the button is held down
    env.SetSustainLevel(1.f);

    //Start the adc
    hardware.adc.Start();

    //Start calling the audio callback
    hardware.StartAudio(AudioCallback);

    // Loop forever
    for(;;) {
    }
}
