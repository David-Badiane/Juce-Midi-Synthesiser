#pragma once

#include <JuceHeader.h>
#include "WaveGenerator.h"
//==============================================================================
/*
*/
class BleepTriangleWaveSound : public WaveGeneratorSound {};

class BleepTriangleWaveVoice : public WaveGeneratorVoice {

public:
	BleepTriangleWaveVoice() : level(0), modulo(0), inc(0), angleDelta(0) {}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int) override 
	{
		adsr.noteOn();
		lastOutput = 0.0;
		level = velocity * 0.25 ;
		modulo = 0.0;
		currentState = 1.0;

		noteFrequency = noteHz(midiNoteNumber, pitchBendCents());
		inc = noteFrequency /getSampleRate();
		angleDelta = double_Pi * inc;
	}

	void recalculatePitch() {
		inc = noteFrequency * std::pow(2.0, pitchBendCents() / 1200) / getSampleRate();
		angleDelta = double_Pi * inc;
	}

	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		adsr.setSampleRate(getSampleRate());
		adsr.setParameters(adsrParameters);
		processBlock(outputBuffer, startSample, numSamples);
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
				currentState = angleDelta * currentState + (1 - angleDelta) * lastOutput;
				lastOutput = currentState;
				float Sample = (float)(currentState * masterGain);

				for (int i = outputBuffer.getNumChannels(); --i >= 0;)
					outputBuffer.addSample(i, startSample, adsr.getNextSample() * Sample);

				modulo += inc;
				++startSample;
		}
	}

	double level, modulo, inc, angleDelta, currentState;
	double lastOutput;
};