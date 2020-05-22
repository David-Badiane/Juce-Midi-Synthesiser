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

		noteFrequency = noteHz(midiNoteNumber, pitchBendCents());
		originalNoteFreq = noteFrequency;
		adsr.noteOn();
		inc = 0;
		currentAngle = 0.0;
		level = velocity * 0.25 ;
		
		inc = noteFrequency / getSampleRate();
		angleDelta = inc * 2.0 * double_Pi;
		
	}

	void recalculatePitch() {
		double frequency = noteFrequency * std::pow(2.0, pitchBendCents() / 1200);
		inc = frequency / getSampleRate();
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

	double currentAngle, angleDelta, level;
};