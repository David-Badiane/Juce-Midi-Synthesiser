#pragma once

#include <JuceHeader.h>
#include "WaveGenerator.h"
//==============================================================================
/*
*/
class SquareWaveSound : public WaveGeneratorSound {};

class SquareWaveVoice : public WaveGeneratorVoice {

public:
	SquareWaveVoice() : level(0), modulo(0), inc(0) {}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int) override {

		adsr.noteOn();
		level = velocity * 0.25;
		modulo = 0.0;
		trigger = 0;
		currentState = 1.0;
		double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber) * 2.0;
		inc = cyclesPerSecond / getSampleRate();
	}


	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		adsr.setSampleRate(getSampleRate());
		adsr.setParameters(adsrParameters);
		processBlock(outputBuffer, startSample, numSamples);
	}



private:

	void processBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
	{
		while (--numSamples >= 0)
		{
			if (modulo >= 1)
			{
				modulo -= 1.0;
				currentState *= (-1.0);
			}
						
		    float Sample = (float)(currentState * level * masterGain);
		    for (int i = outputBuffer.getNumChannels(); --i >= 0;) {
		    	outputBuffer.addSample(i, startSample, adsr.getNextSample() * Sample);
		    }
		    
		    modulo += inc;
		    ++startSample;
		}
	}

	double level, modulo, currentState, inc;
	int trigger;
};
