#pragma once

#include <JuceHeader.h>
#include "WaveGenerator.h"
//==============================================================================
/*
*/
class SineBeatsWaveSound : public WaveGeneratorSound {
};

class SineBeatsWaveVoice : public WaveGeneratorVoice {

public:
	SineBeatsWaveVoice() : currentAngle{ 0,0 }, angleDelta{ 0, 0 }, level(0), noteFrequency(0), deltaFreq(0) {}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int) override 
	{
		adsr.noteOn();
		deltaFreq = 4;
		noteFrequency = noteHz(midiNoteNumber, pitchBendCents());
		for (int i = 0; i < 2; i++)
		{
			currentAngle[i] = 0;
			double frequency = noteFrequency + i*deltaFreq;
			double cyclesPerSample = frequency / getSampleRate();
			angleDelta[i] = cyclesPerSample * 2.0 * double_Pi;
		}
		level = velocity * 0.15;
	}

	void update_beats(double deltaFreqParam)
	{
		deltaFreq = deltaFreqParam;
		for (int i = 0; i < 2; i++)
		{
			double frequency = noteFrequency + i * deltaFreq;
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

			for (int i = 0; i < 2; i++)
				Sample = Sample +(float)(std::sin(currentAngle[i]) * level * masterGain);
;

			for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
				outputBuffer.addSample(i, startSample, adsr.getNextSample() * Sample);

			for (int i = 0; i < 2; i++)
				currentAngle[i] += angleDelta[i];

			++startSample;
	
		}
	}

	double currentAngle[2], angleDelta[2],level, deltaFreq, noteFrequency;
};