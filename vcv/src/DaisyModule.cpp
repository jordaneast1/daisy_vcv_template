#include "plugin.hpp"
#include "SharedDSP.h"


struct DaisyModule : Module {
	// All of the DSP lives in ../../shared, shared with the Daisy firmware.
	// Everything in this file is VCV front panel.
	shared::SharedDSP voice;

	enum ParamId {
		PITCH_PARAM,
		GATE_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		GATE_LIGHT,
		LIGHTS_LEN
	};

	DaisyModule() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		// 0..1 is the Daisy ADC's range, so the knob lands on the same pitch on both.
		configParam(PITCH_PARAM, 0.f, 1.f, 0.5f, "Pitch");
		configButton(GATE_PARAM, "Gate");
		configOutput(OUTPUT, "Sine");

		voice.Init(APP->engine->getSampleRate());
	}

	void onSampleRateChange(const SampleRateChangeEvent& e) override {
		voice.Init(e.sampleRate);
	}

	void process(const ProcessArgs& args) override {
		// Stands in for the momentary button wired to the Daisy
		bool gate = params[GATE_PARAM].getValue() > 0.f;

		float sig = voice.Process(params[PITCH_PARAM].getValue(), gate);

		// Audio signals are typically +/-5V
		// https://vcvrack.com/manual/VoltageStandards
		outputs[OUTPUT].setVoltage(5.f * sig);

		// The Daisy lights its onboard LED while the button is held; do the same here.
		lights[GATE_LIGHT].setBrightness(gate ? 1.f : 0.f);
	}
};


struct DaisyModuleWidget : ModuleWidget {
	DaisyModuleWidget(DaisyModule* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/DaisyModule.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.24, 46.063)), module, DaisyModule::PITCH_PARAM));

		addParam(createParamCentered<VCVButton>(mm2px(Vec(15.24, 77.478)), module, DaisyModule::GATE_PARAM));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 108.713)), module, DaisyModule::OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(15.24, 25.81)), module, DaisyModule::GATE_LIGHT));
	}
};


Model* modelDaisyModule = createModel<DaisyModule, DaisyModuleWidget>("DaisyModule");
