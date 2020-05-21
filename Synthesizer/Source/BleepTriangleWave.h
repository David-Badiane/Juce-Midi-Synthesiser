#pragma once

#include <JuceHeader.h>
#include "WaveGenerator.h"
//==============================================================================
/*
*/
class BleepTriangleWaveSound : public WaveGeneratorSound {};

class BleepTriangleWaveVoice : public WaveGeneratorVoice {

public:
	BleepTriangleWaveVoice() : level(0), modulo(0){}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int) override 
	{
		adsr.noteOn();
		level = velocity * 0.25 ;
		modulo = 0.0;
		noteFrequency = noteHz(midiNoteNumber, pitchBendCents());
		inc = 2 * noteFrequency /getSampleRate();
	}


	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		adsr.setSampleRate(getSampleRate());
		adsr.setParameters(adsrParameters);
		processBlock(outputBuffer, startSample, numSamples);
	}

private:

	double polyBleep(double t)
	{
		if (t < inc)
		{
			t /= inc;
			return t + t - t * t - 1.0;
		}
		else if (t > 2.0 - inc)
		{
			t = (t - 2.0) / inc;
			return t * t + t + t + 1.0;
		}
		else return 0.0;
	}

	void processBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
	{
		while (--numSamples >= 0)
		{
			if (modulo >= 2.0)
				modulo -= 2.0;
			double normalTriangle = (abs(2 * modulo - 2.0) -1) * level;
			double polyTriangle = normalTriangle - polyBleep(modulo);
			float sample = (float)(polyTriangle * masterGain);
			for (int i = outputBuffer.getNumChannels(); --i >= 0;)
				outputBuffer.addSample(i, startSample, adsr.getNextSample() * sample);
			modulo += inc;
			++startSample;
		}
	}

	double level, modulo;
};