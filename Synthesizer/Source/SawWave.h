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
		level = velocity * 0.25;
		modulo = 0.0;
		double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		inc = cyclesPerSecond / getSampleRate();
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
			if (modulo >= 1.0)
			{
				modulo -= 1.0;
				const FloatType currentSample = static_cast<FloatType> (modulo * 2.0 - 1.0) * level;

				for (int i = outputBuffer.getNumChannels(); --i >= 0;)
					outputBuffer.addSample(i, startSample, adsr.getNextSample() * currentSample);


				modulo += inc;
				++startSample;
			}
			else
			{
				const FloatType currentSample =
					static_cast<FloatType> (modulo * 2.0 - 1.0) * level;

				for (int i = outputBuffer.getNumChannels(); --i >= 0;)
					outputBuffer.addSample(i, startSample, adsr.getNextSample() * currentSample);

				modulo += inc;
				++startSample;

			}
		}
	}

	double level, modulo, inc;

};