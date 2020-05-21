#pragma once

#include <JuceHeader.h>
#include "WaveGenerator.h"
//==============================================================================
/*
*/
class WhiteNoiseWaveSound : public WaveGeneratorSound {};

class WhiteNoiseWaveVoice : public WaveGeneratorVoice {

public:
	WhiteNoiseWaveVoice() : currentAngle(0), angleDelta(0), level(0) {}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int) override {

		adsr.noteOn();
		level = velocity * 0.15 ;
		currentAngle = 0.0;

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
		inc = noteFrequency * std::pow(2.0, pitchBendCents() / 1200) / getSampleRate();
		angleDelta = inc * 2.0 * double_Pi;
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
			const float Sample = (float)(((rnumber.nextFloat() ) - 1.0 + std::sin(currentAngle)) * level * masterGain);
			for (int i = outputBuffer.getNumChannels(); --i >= 0;) {
				outputBuffer.addSample(i, startSample, adsr.getNextSample() * Sample);
			}
			currentAngle += angleDelta;
			++startSample;
		}
	}

	double currentAngle, angleDelta,level;
	Random rnumber;
};