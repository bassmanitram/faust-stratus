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
		const char* stratusIdKey = "stratusId";
		const char* versionKey = "version";
	protected:
		std::string effectName;
		std::string effectId;
		std::string version;
	public:
		void declare(const char* key, const char* value) {
			if (strcmp(key,nameKey) == 0) {
				effectName.assign(value);
			} else if (strcmp(key,stratusIdKey) == 0) {
				effectId.assign(value);
			} else if (strcmp(key,versionKey) == 0) {
				version.assign(value);
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
		std::string effectName;
		std::string effectId;
		std::string version;
		UI* faustUi;
		Meta* faustMeta;
		FAUSTCLASS* faust;

	public:
		int fSampleRate = 44100;
		Stratus()
		{
			faustUi = new UI;
			faustMeta = new Meta;
			faust = new FAUSTCLASS;
			faust->metadata(faustMeta);
			effectId = faustMeta->effectId;
			effectName = faustMeta->effectName;
			version = faustMeta->version;

			faust->init(fSampleRate);
			faust->buildUserInterface(faustUi);

			stompSwitch = DOWN;
		}
		~Stratus() {}

		void setName(std::string name)
		{
			printf("HEY THERE\n");
			this->effectName = name;
		}

		std::string getName()
		{
			return this->effectName;
		}

		const char* getNameC()
		{
			return this->effectName.c_str();
		}

		/* I could not get .c_str() to work in the C interface code for this getter, so had to create another method to do it */
		const char* getEffectIdC()
		{
			return this->effectId.c_str();
		}

		std::string getEffectId()
		{
			//printf("In getter: %s\n",this->effectId);
			return this->effectId;
		}

		const char * getVersionC()
		{
			return this->version.c_str();
		}

		std::string getVersion()
		{
			return this->version;
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
	const char* stratusGetEffectId(Stratus* stratus){return stratus->getEffectIdC();}
	const char* stratusGetName(Stratus* stratus){return stratus->getNameC();}
	const char* stratusGetVersion(Stratus* stratus){return stratus->getVersionC();}

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
