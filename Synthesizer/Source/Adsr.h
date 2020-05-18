#ifndef ADSR_H_INCLUDED
#define ADSR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//Funkcje inline - kompilator zamiast linkowac wskaznik do definicji funkcji, wkleja jej zawartosc w miejscu
//				   wystapienia. Ma to zastosowanie, gdy programiscie zalezy na szybkosci dzialania programu.
//				   Funkcja skladowa klasy zostanie automatycznie uznana za inline, gdy umiescimy jej kod zaraz
//				   po jej deklaracji we wnetrzu klasy

class Adsr
{
public:
	Adsr() :
		minimumLevel(0.001),
		currentState(OFF),
		currentLevel(minimumLevel),
		multiplier(1.0),
		sampleRate(44100),
		currentSampleIndex(0),
		nextStateSampleIndex(0)
	{}

	~Adsr()
	{}

	enum EnvelopeState
	{
		OFF = 0,
		ATTACK,
		DECAY,
		SUSTAIN,
		RELASE,
		kNumEnvelopeStates //informuje o ilosci stanow obwiedni (5)
	};

	void setAttack(double attack)
	{
		stateValue[ATTACK] = attack;
	}
	void setDecay(double decay)
	{
		stateValue[DECAY] = decay;
	}
	void setSustain(double sustain)
	{
		stateValue[SUSTAIN] = sustain;
	}
	void setRelease(double relase)
	{
		stateValue[RELASE] = relase;
	}

	void enterState(EnvelopeState newState);
	double nextSample();
	void setSampleRate(double newSampleRate);
	inline EnvelopeState getCurrentState() const { return currentState; }
	const double minimumLevel; // potrzebne poniewaz kod do obliczania wartosci obwiedni nie dziala z zerem



	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples, MidiBuffer& midiMessages, double proportion)
	{
		processBlock(outputBuffer, startSample, numSamples, midiMessages, proportion);
	}


private:
	template <typename FloatType>
	void processBlock(AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples, MidiBuffer& midiMessages, double proportion)
	{
		while (--numSamples >= 0)
		{
			double envelopeMultiplier = nextSample();

			for (int i = outputBuffer.getNumChannels(); --i >= 0;)
			{
				double currentSample = outputBuffer.getSample(i, startSample);
				double product = currentSample * envelopeMultiplier;
				const FloatType sample = static_cast<FloatType> (product);
				outputBuffer.setSample(i, startSample, sample * proportion + (1.0 - proportion) * currentSample);
			}
			++startSample;
		}
	}

	void calculateMultiplier(double startLevel, double endLevel, unsigned long long lengthInSamples);
	EnvelopeState currentState;
	double currentLevel;
	double multiplier;
	double sampleRate;
	double stateValue[kNumEnvelopeStates];
	unsigned long long currentSampleIndex;
	unsigned long long nextStateSampleIndex;

};



#endif 