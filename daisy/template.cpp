#include "daisy_seed.h"
#include "SharedDSP.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;

// Declare a DaisySeed object called hardware
DaisySeed hardware;

//Configure and initialize button
Switch button1;

// All of the DSP lives in ../shared, so the VCV Rack module can run the
// same voice. Everything in this file is Daisy hardware "front panel".
shared::SharedDSP voice;

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    //Nobody likes a bouncy button
    button1.Debounce();

    //Hold the button to hold the envelope at its sustain level
    bool gate = button1.Pressed();

    //Light the onboard LED while the button is held down
    hardware.SetLed(gate);

    //Read the knob, which the shared voice maps to pitch
    float pitch_knob = hardware.adc.GetFloat(0);

    //Fill the block with samples
    for(size_t i = 0; i < size; i += 2)
    {
        float sig = voice.Process(pitch_knob, gate);

        //Set the left and right outputs
        out[i]     = sig;
        out[i + 1] = sig;
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

    //Set up the shared oscillator + envelope
    voice.Init(samplerate);

    //Start the adc
    hardware.adc.Start();

    //Start calling the audio callback
    hardware.StartAudio(AudioCallback);

    // Loop forever
    for(;;) {
    }
}
