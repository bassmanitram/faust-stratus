/*
 Architecture wrapper for the Chaos Stratus pedal/platform
*/

/******************* BEGIN stratus.cpp ****************/

#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cmath>
#include <math.h>

#include <iostream>
#include <cstdio>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#define MAXKNOBS 10
#define MAXSWITCHES 5

#define virtual

#ifndef Uint
typedef unsigned int Uint;
#endif

//using namespace std;
//
// Our implementation of the Faust Meta interface
//
class Meta
{
	private:
		const char* nameKey = "name";
	protected:
		const char* name;
	public:
		void declare(const char* key, const char* value) {
			if (strcmp(key,nameKey) == 0) {
				name = value;
			}
		}
	friend class Stratus;
};

//
// Our implementation of the Faust UI interface
//
// hsliders and vsliders are knobs
// checkboxes, and nentries with min=0, max=1 or 2, and step=1 are switches
//

class UI {
	private:
	    void add_knob(FAUSTFLOAT* slider) {
			if (knobCount < MAXKNOBS) {
				knobs[knobCount] = slider;
				knobCount++;
			}
		}

	    void add_switch(FAUSTFLOAT* swtch) {
			if (switchCount < MAXSWITCHES) {
				switches[switchCount] = swtch;
				switchCount++;
			}
		}

	protected:
		FAUSTFLOAT* switches[MAXSWITCHES];
		Uint switchCount = 0;
		FAUSTFLOAT* knobs[MAXKNOBS];
		Uint knobCount = 0;

	public:
		UI() {}
		~UI();
	    void openTabBox(const char* label) {};
	    void openHorizontalBox(const char* label) {};
	    void openVerticalBox(const char* label) {};
	    void closeBox() {};
	    void addButton(const char* label, FAUSTFLOAT* zone) {
			this->add_switch(zone);
		};
	    void addCheckButton(const char* label, FAUSTFLOAT* zone) {
			this->add_switch(zone);
		};
	    void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
//			printf("VSLIDER: %s %p %f %f %f %f\n",label,zone,init,min,max,step);
			this->add_knob(zone);
		};
	    void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
//			printf("HSLIDER: %s %p %f %f %f %f\n",label,zone,init,min,max,step);
			this->add_knob(zone);
		};
	    void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
			FAUSTFLOAT steps = (max - min)/step;
			if (min == 0 && (max == 1 || max == 2) && step == 1) {
				this->add_switch(zone);
			}
		};
	    void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {};
	    void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {};
	    void addSoundfile(const char* label, const char* filename, void** sf_zone) {};
	    void declare(FAUSTFLOAT* slider, const char* key, const char* val) {}

	friend class Stratus;
};

class dsp {};

/******************************************************************************
 ******************************************************************************
 
 VECTOR INTRINSICS
 
 *******************************************************************************
 *******************************************************************************/

<<includeIntrinsic>>

/********************END ARCHITECTURE SECTION (part 1/2)****************/

/**************************BEGIN USER SECTION **************************/

<<includeclass>>

/***************************END USER SECTION ***************************/

/*******************BEGIN ARCHITECTURE SECTION (part 2/2)***************/

//
// The interface to the stratus - very similar to Faust, but not exactly the
// the same, so the default implementation is a "protocol converter" between
// the two, allowing the Faust code to be used as-is.
//
// HOWEVER - the Faust code itself may need tweaking to properly implement the
// algorithm that it represents - THIS code is simply how to use Faust code
// 'directly' in the stratus.
//
struct Stratus
{
	enum SWITCH_STATE
	{
		UP = 0,
		DOWN = 1,
		MIDDLE = 2
	};

	static SWITCH_STATE switchStateFromValue(int value) {
		switch (value) {
    		case 0: return SWITCH_STATE::UP;
    		case 1: return SWITCH_STATE::DOWN;
    		case 2: return SWITCH_STATE::MIDDLE;
    		default: return SWITCH_STATE::DOWN;
		}
	}

	// Use for switch debugging
	void getTextForEnum(SWITCH_STATE enumVal, std::string *out) {
		switch (enumVal) {
			case 0:  *out = "UP";
			case 1:  *out = "DOWN";
			case 2:  *out = "MIDDLE";
			default: *out = "BAD";
		}
		return;
	}

	private:
		SWITCH_STATE stompSwitch;
		std::string name;
		UI* faustUi;
		Meta* faustMeta;
		FAUSTCLASS* faust;

	protected:
		std::string version;

	public:
		int fSampleRate = 44100;
		Stratus()
		{
//			std::cout << "INITIALIZING" << std::endl;
			faustUi = new UI;
//			std::cout << "UI created" << std::endl;
			faustMeta = new Meta;
//			std::cout << "Meta created" << std::endl;
			faust = new FAUSTCLASS();
//			std::cout << "Faust created" << std::endl;
			faust->metadata(faustMeta);
//			std::cout << "Faust metadata retrieved" << std::endl;
			faust->init(fSampleRate);
//			std::cout << "Faust initialized" << std::endl;
			faust->buildUserInterface(faustUi);
//			std::cout << "Faust UI interrogated" << std::endl;

			name = faustMeta->name;
//			std::cout << "Name retrieved: " << name << std::endl;
			stompSwitch = DOWN;
//			std::cout << "INITIALIZED " << name << std::endl;
		}
		~Stratus() {}

		void setName(std::string name)
		{
			this->name = name;
		}

		std::string getName()
		{
			return name;
		}

		std::string getVersion()
		{
			return version;
		}

		Uint getKnobCount() {
			return this->faustUi->knobCount;
		}

		void setKnob(int num, float knobVal)
		{
			if (num < this->faustUi->knobCount) {
				*(this->faustUi->knobs[num]) = knobVal;
			}
		}

		float getKnob(int in)
		{
			return in < this->faustUi->knobCount ? *this->faustUi->knobs[in] : 0.5;
		}

		Uint getSwitchCount() {
			return this->faustUi->switchCount;
		}

		void setSwitch(int num, SWITCH_STATE switchVal)
		{
			if (num < this->faustUi->switchCount) {
				*(this->faustUi->switches[num]) = switchVal;
			}
		}

		SWITCH_STATE getSwitch(int in)
		{
			Uint switchVal = in < this->faustUi->switchCount ? *this->faustUi->switches[in] : 0;
			return switchStateFromValue(switchVal < 3 ? switchVal : 0);
		}

		void setStompSwitch(SWITCH_STATE switchVal)
		{
			stompSwitch = switchVal;
		}

		bool getStompSwitch()
		{
			return stompSwitch;
		}

		void stompSwitchPressed(int count, FAUSTFLOAT *inputs, FAUSTFLOAT *outputs)
		{
			if (stompSwitch)
			{
				compute(count, inputs, outputs);
			}
			return;
		}

		void compute(int count, FAUSTFLOAT *inputs, FAUSTFLOAT *outputs) {
			this->faust->compute(count,&inputs,&outputs);
		}
};

extern "C" {
	Stratus* create() {	return new Stratus;}
#ifdef CINTERFACE
	//
	// Expose the C inteface to allow for, say, Python texting
	//
	void  stratusSetName(Stratus* stratus, char* name) { stratus->setName(name); }
	const char* stratusGetName(Stratus* stratus) {return stratus->getName().c_str();}
	const char* stratusGetVersion(Stratus* stratus){return stratus->getVersion().c_str();}
	Uint   stratusGetKnobCount(Stratus* stratus){return stratus->getKnobCount();}
	Uint   stratusGetSwitchCount(Stratus* stratus){return stratus->getSwitchCount();}
	void  stratusSetKnob(Stratus* stratus, Uint num, float knobVal){stratus->setKnob(num, knobVal);}
	float stratusGetKnob(Stratus* stratus, Uint in){return stratus->getKnob(in);}
	void  stratusSetSwitch(Stratus* stratus, Uint num, int switchVal){stratus->setSwitch(num, (Stratus::SWITCH_STATE)switchVal);}
	Uint   stratusGetSwitch(Stratus* stratus, Uint in) {return stratus->getSwitch(in);}
	void  stratusSetStompSwitch(Stratus* stratus, Uint switchVal) {stratus->setStompSwitch((Stratus::SWITCH_STATE)switchVal);}
	bool  stratusGetStompSwitch(Stratus* stratus) {return stratus->getStompSwitch();}
	void  stratusStompSwitchPressed(Stratus* stratus, Uint count, FAUSTFLOAT *inputs, FAUSTFLOAT *outputs){stratus->stompSwitchPressed(count, inputs, outputs);}
	void  stratusCompute(Stratus* stratus, Uint count, FAUSTFLOAT *inputs, FAUSTFLOAT *outputs) {stratus->compute(count, inputs, outputs);}
#endif
}

using dsp_creator_t = Stratus *(*)();

/******************* END stratus.cpp ****************/
