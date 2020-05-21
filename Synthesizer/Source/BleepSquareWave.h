#pragma once

#include <JuceHeader.h>
#include "WaveGenerator.h"
//==============================================================================
/*
*/
class BleepSquareWaveSound : public WaveGeneratorSound {};

class BleepSquareWaveVoice : public WaveGeneratorVoice {

public:
	BleepSquareWaveVoice() : level(0), modulo(0), inc(0) {}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int) override {

		adsr.noteOn();
		level = velocity * 0.25 ;
		modulo = 0.0;
	
		noteFrequency = noteHz(midiNoteNumber, pitchBendCents());
		inc = noteFrequency / getSampleRate();
	}

	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		adsr.setSampleRate(getSampleRate());
		adsr.setParameters(adsrParameters);
		processBlock(outputBuffer, startSample, numSamples);
	}

	void recalculatePitch() {
		inc = noteFrequency * std::pow(2.0, pitchBendCents() / 1200) / getSampleRate();
	}

private:

	double polyBlep(double t)
	{
		if (t < inc)
			// 0 <= t < 1
		{
			t /= inc;
			return t + t - t * t - 1.0;
		}
		// -1 < t < 0
		else if (t > 1.0 - inc)
		{
			t = (t - 1.0) / inc;
			return t * t + t + t + 1.0;
		}
		//0 
		else return 0.0;
	}

	void processBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
	{
		while (--numSamples >= 0)
		{
			if (modulo >= 1.0)
					modulo -= 1.0;

			if (modulo > 0.5)
				currentState = -1.0;
			else
				currentState = 1.0;
		
			currentState *= level;
			currentState += polyBlep(modulo);
			currentState -= polyBlep(fmod(modulo + 0.5, 1.0));
			float Sample = static_cast<float>(currentState * masterGain);

			for (int i = outputBuffer.getNumChannels(); --i >= 0;)
				outputBuffer.addSample(i, startSample, adsr.getNextSample() * Sample);

			modulo += inc;
				
			++startSample;
		}
	}

	double level, modulo, inc, angleDelta, currentAngle, currentState, lastOutput;
};