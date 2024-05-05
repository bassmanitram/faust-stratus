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
// Effectively we ignore everything except sliders at the moment
//

class UI {
	private:
	    void add_knob(FAUSTFLOAT* slider) {
			if (knobCount < MAXKNOBS) {
				knobs[knobCount] = slider;
				knobCount++;
			}
		}

	protected:
		FAUSTFLOAT* knobs[MAXKNOBS];
		Uint knobCount = 0;

	public:
		UI() {}
		~UI();
	    void openTabBox(const char* label) {};
	    void openHorizontalBox(const char* label) {};
	    void openVerticalBox(const char* label) {};
	    void closeBox() {};
	    void addButton(const char* label, FAUSTFLOAT* zone) {};
	    void addCheckButton(const char* label, FAUSTFLOAT* zone) {};
	    void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
//			printf("VSLIDER: %s %p %f %f %f %f\n",label,zone,init,min,max,step);
			this->add_knob(zone);
		};
	    void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
//			printf("HSLIDER: %s %p %f %f %f %f\n",label,zone,init,min,max,step);
			this->add_knob(zone);
		};
	    void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {};
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

	private:
		SWITCH_STATE switches[MAXSWITCHES];
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
			for (int i = 0; i < MAXSWITCHES; ++i)
				switches[i] = SWITCH_STATE::DOWN;
			stompSwitch = DOWN;
//			std::cout << "INITIALIZED " << name << std::endl;
		}
		~Stratus() {}

		// Use for switch debugging
		// static const int SWITCH_STATES = 3;
		// const char* swStates[SWITCH_STATES] = {"UP", "MIDDLE", "DOWN"};

		void getTextForEnum(SWITCH_STATE enumVal, std::string *out)
		{
			if (enumVal == 0) // SWITCH_STATE::UP)
				*out = "UP";
			else if (enumVal == 1) // SWITCH_STATE::MIDDLE)
				*out = "MIDDLE";
			else if (enumVal == 2) // SWITCH_STATE::DOWN)
				*out = "DOWN";
			else
				*out = "BAD";
			return;
		}

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

		void setSwitch(int num, SWITCH_STATE switchVal)
		{
			switches[num] = switchVal;
		}

		SWITCH_STATE getSwitch(int in)
		{
			return switches[in];
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
