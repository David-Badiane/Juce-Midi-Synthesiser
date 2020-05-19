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
		level = velocity * 0.25;
		double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		double cyclesPerSample = cyclesPerSecond / getSampleRate();
		angleDelta = cyclesPerSample * 2.0 * double_Pi;
	}



	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		adsr.setSampleRate(getSampleRate());
		adsr.setParameters(adsrParameters);
		processBlock(outputBuffer, startSample, numSamples);
	}

private :
	
	template <typename FloatType>

	void processBlock(AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples) 
	{
	
		while (--numSamples >= 0)
		{
			auto currentSample = (float)(std::sin(currentAngle) * level);

			for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
				outputBuffer.addSample(i, startSample, adsr.getNextSample() * currentSample);

			currentAngle += angleDelta;
			++startSample;
		}
	
	
	}

	double currentAngle, angleDelta, level;
};