#pragma once

#include <JuceHeader.h>
#include "WaveGenerator.h"
//==============================================================================
/*
*/
class SineWaveSound : public WaveGeneratorSound{
};

class SineWaveVoice : public WaveGeneratorVoice{

public:
	SineWaveVoice() : currentAngle(0), angleDelta(0), level(0) {}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int) override{

		adsr.noteOn();
		currentAngle = 0.0;
		level = velocity * 0.25 ;
		noteFrequency = noteHz(midiNoteNumber, pitchBendCents());
		inc = noteFrequency / getSampleRate();
		angleDelta = inc * 2.0 * double_Pi;
		
	}


	void stretchFrequencies() {
		int sign = 1;
		if (noteFrequency > 10000) {
			decrescentmod = true;
		}
		else if (noteFrequency < 40) {
			decrescentmod = false;
		}
		if (decrescentmod) {
			sign = -1;
		}

		noteFrequency *= std::pow(2.0, 64 * modWheel * sign / 1200); //change pitchBendCents() with modwheel
		inc = noteFrequency / getSampleRate();
	}


	void recalculatePitch() {
		double frequency = noteFrequency * std::pow(2.0, pitchBendCents() / 1200);
		inc = frequency / getSampleRate();
		stretchFrequencies();
		angleDelta = inc * 2.0 * double_Pi;
	}


	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		adsr.setSampleRate(getSampleRate());
		adsr.setParameters(adsrParameters);
		processBlock(outputBuffer, startSample, numSamples);
	}

private :

	void processBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) 
	{
	
		while (--numSamples >= 0)
		{

			auto Sample = (float)(std::sin(currentAngle) * std::pow(2, level * masterGain));
			
			for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
				outputBuffer.addSample(i, startSample, adsr.getNextSample() * Sample);

			currentAngle += angleDelta;
			++startSample;
		}
	}

	double currentAngle, angleDelta, level, inc;
};