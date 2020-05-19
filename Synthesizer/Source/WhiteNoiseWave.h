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
		level = velocity * 0.15;
		currentAngle = 0.0;

		double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		inc = cyclesPerSecond / getSampleRate();
		angleDelta = inc * 2.0 * double_Pi;
	}

	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{	
		adsr.setSampleRate(getSampleRate());
		adsr.setParameters(adsrParameters);
		processBlock(outputBuffer, startSample, numSamples);
	}

private:

	template <typename FloatType>
	void processBlock(AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples)
	{
		while (--numSamples >= 0)
		{
			const FloatType currentSample = static_cast<FloatType> (((rnumber.nextFloat() ) - 1.0 + std::sin(currentAngle)) * level);
			for (int i = outputBuffer.getNumChannels(); --i >= 0;) {
				outputBuffer.addSample(i, startSample, adsr.getNextSample() * currentSample);
			}
			currentAngle += angleDelta;
			++startSample;
		}
	}

	double currentAngle, angleDelta,level, inc;
	Random rnumber;
};