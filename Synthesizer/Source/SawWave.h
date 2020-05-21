#pragma once

#include <JuceHeader.h>
#include "WaveGenerator.h"
//==============================================================================
/*
*/
class SawWaveSound : public WaveGeneratorSound {};

class SawWaveVoice : public WaveGeneratorVoice {

public:
	SawWaveVoice() : level(0), modulo(0), inc(0) {}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int) override {

		adsr.noteOn();
		level = velocity * 0.25 ;
		modulo = 0.0;
		noteFrequency = noteHz(midiNoteNumber, pitchBendCents());
		double cyclesPerSecond = noteFrequency;
		inc = noteFrequency / getSampleRate();
	}

	void recalculatePitch() {
		inc = noteFrequency * std::pow(2.0, pitchBendCents() / 1200) / getSampleRate();
		stretchFrequencies();
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
			if (modulo >= 1.0)
				modulo -= 1.0;

			float Sample = static_cast<float> (modulo * 2.0 - 1.0) * level * masterGain;

			for (int i = outputBuffer.getNumChannels(); --i >= 0;)
					outputBuffer.addSample(i, startSample, adsr.getNextSample() * Sample);

			modulo += inc;
			++startSample;
		}
	}

	double level, modulo, inc, deltaFreq;

};