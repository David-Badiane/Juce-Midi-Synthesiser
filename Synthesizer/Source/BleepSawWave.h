#pragma once

#include <JuceHeader.h>
#include "WaveGenerator.h"
//==============================================================================
/*
*/
class BleepSawWaveSound : public WaveGeneratorSound {};

class BleepSawWaveVoice : public WaveGeneratorVoice {

public:
	BleepSawWaveVoice() : level(0), modulo(0), inc(0) {}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int) override {

		adsr.noteOn();
		level = velocity * 0.25 ;
		modulo = 0.0;
		
		//double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		noteFrequency = noteHz(midiNoteNumber, pitchBendCents());
		inc = noteFrequency / getSampleRate();
	}

	void recalculatePitch() {
		inc = noteFrequency * std::pow(2.0, pitchBendCents() / 1200) / getSampleRate();
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

			double Sample = (modulo * 2.0 - 1.0) * level;
			double value = Sample - polyBleep(modulo);
			float sample = float(value * masterGain);


			for (int i = outputBuffer.getNumChannels(); --i >= 0;)
				outputBuffer.addSample(i, startSample, adsr.getNextSample() * sample);

			modulo += inc;
			++startSample;

		}
	}

	double level, modulo, inc;
};