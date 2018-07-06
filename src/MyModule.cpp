#include "Template.hpp"
#include "dsp/digital.hpp"
#include "formula/Formula.h"

struct FrankBussFormulaModule;

class MyTextField : public LedDisplayTextField {
public:
	MyTextField() : LedDisplayTextField() {}
	void setModule(FrankBussFormulaModule* _module) {
		module = _module;
	}
	virtual void onTextChange() override;

private:
	FrankBussFormulaModule* module;
};

struct FrankBussFormulaModule : Module {
	enum ParamIds {
		X_PARAM,
		Y_PARAM,
		Z_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		X_INPUT,
		Y_INPUT,
		Z_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		FORMULA_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		BLINK_LIGHT,
		NUM_LIGHTS
	};

	Formula formula;
	MyTextField* textField;
	bool compiled = false;
	float phase = 0.0;
	float blinkPhase = 0.0;

	FrankBussFormulaModule() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
	}

	void step() override;

	void onCreate () override
	{
		// textField->text = "x + y + z";
		compiled = false;
		if (textField->text.size() > 0) {
			try {
				formula.setExpression(textField->text);
				formula.setVariable("pi", M_PI);
				compiled = true;
			} catch (exception&) {
				// printf("Exception: %s\n", exc.what());
			}
		}
	}

	void onReset () override
	{
		onCreate();
	}

};

void MyTextField::onTextChange() {
	module->onCreate();
}

void FrankBussFormulaModule::step() {
	float deltaTime = engineGetSampleTime();

	// get inputs
	float x = inputs[X_INPUT].value;
	float y = inputs[Y_INPUT].value;
	float z = inputs[Z_INPUT].value;

	// calculate formula
	float val = 0;
	if (compiled) {
		try {
			formula.setVariable("x", x);
			formula.setVariable("y", y);
			formula.setVariable("z", z);
			val = formula.eval();
			if (!isfinite(val) || isnan(val)) val = 0.0f;
			val = clamp(val, -5.0f, 5.0f);
		} catch (MathError&) {
			// ignore math errors, e.g. division by zero
		} catch (exception&) {
			// for all other exceptions, set compiled to false, e.g. VariableNotFound
			compiled = false;
		}
	}

	// set output
	outputs[FORMULA_OUTPUT].value = val;

	// Blink light at 1Hz
	blinkPhase += deltaTime;
	if (blinkPhase >= 1.0f)
		blinkPhase -= 1.0f;
	if (compiled)
	{
		lights[BLINK_LIGHT].value = 1.0f;
	}
	else
	{
		lights[BLINK_LIGHT].value = (blinkPhase < 0.5f) ? 1.0f : 0.0f;
	}
}


struct FrankBussFormulaWidget : ModuleWidget {
	MyTextField *textField;
	FrankBussFormulaWidget(FrankBussFormulaModule *module) : ModuleWidget(module) {

		setPanel(SVG::load(assetPlugin(plugin, "res/MyModule.svg")));

		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(Port::create<PJ301MPort>(Vec(38, 260), Port::INPUT, module, FrankBussFormulaModule::X_INPUT));
		addInput(Port::create<PJ301MPort>(Vec(98, 260), Port::INPUT, module, FrankBussFormulaModule::Y_INPUT));
		addInput(Port::create<PJ301MPort>(Vec(153, 260), Port::INPUT, module, FrankBussFormulaModule::Z_INPUT));
		addOutput(Port::create<PJ301MPort>(Vec(215, 260), Port::OUTPUT, module, FrankBussFormulaModule::FORMULA_OUTPUT));

		addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec(125, 100), module, FrankBussFormulaModule::BLINK_LIGHT));
		textField = Widget::create<MyTextField>(mm2px(Vec(3, 42)));
		textField->setModule(module);
		textField->box.size = mm2px(Vec(85, 40));
		textField->multiline = true;

		addChild(textField);
		module->textField = this->textField;
	}
	json_t *toJson() override {
		json_t *rootJ = ModuleWidget::toJson();

		// text
		json_object_set_new(rootJ, "text", json_string(textField->text.c_str()));

		return rootJ;
	}

	void fromJson(json_t *rootJ) override {
		ModuleWidget::fromJson(rootJ);

		// text
		json_t *textJ = json_object_get(rootJ, "text");
		if (textJ)
		{
			textField->text = json_string_value(textJ);
			module->onCreate();
		}
	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelFrankBussFormula = Model::create<FrankBussFormulaModule, FrankBussFormulaWidget>("Frank Buss", "FrankBussFormula", "Formula", UTILITY_TAG);
