#pragma once

#include <JuceHeader.h>
#include "WaveGenerator.h"
//==============================================================================

class OrganWaveSound : public WaveGeneratorSound {
};

class OrganWaveVoice : public WaveGeneratorVoice {

public:
	OrganWaveVoice() : currentAngle{ 0,0,0,0,0,0,0,0,0,0 }, angleDelta{ 0,0,0,0,0,0,0,0,0,0 }, 
					   level(0), noteFrequency(0), deltaFreq(0) 
					   {}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int) override
	{
		adsr.noteOn();
		noteFrequency = noteHz(midiNoteNumber, pitchBendCents());
		for (int i = 0; i < 3; i++)
		{
			currentAngle[i] = 0;
			double frequency = (2^i ) * noteFrequency + i * deltaFreq;
			double cyclesPerSample =  frequency / getSampleRate();
			angleDelta[i] = cyclesPerSample * 2.0 * double_Pi;
		}
		level = velocity * 0.04 ;
	}

	void update_beats(double deltaFreqParam)
	{
		deltaFreq = deltaFreqParam/4;
		for (int i = 0; i < 10; i++)
		{
			double frequency = (2 ^ i) * noteFrequency + i * deltaFreq;
			double cyclesPerSample = frequency / getSampleRate();
			angleDelta[i] = cyclesPerSample * 2.0 * double_Pi;
		}
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
			float Sample = 0;

			for (int i = 0; i < 10; i++)
				Sample = Sample + (float)(std::sin(currentAngle[i]) * level * masterGain);
			;

			for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
				outputBuffer.addSample(i, startSample, adsr.getNextSample() * Sample);

			for (int i = 0; i < 10; i++)
				currentAngle[i] += angleDelta[i];

			++startSample;

		}
	}

	double currentAngle[10], angleDelta[10], level, deltaFreq, noteFrequency;
};