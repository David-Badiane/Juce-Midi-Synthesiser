#ifndef SAWWAVE_H
#define SAWWAVE_H

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../JuceLibraryCode/JuceHeader.h"

class SawWaveSound : public SynthesiserSound
{
public:
	SawWaveSound() {}

	virtual bool appliesToNote(int /*midi note nmbr*/)
	{
		return true;
	}

	virtual bool appliesToChannel(int /*midi channel nmbr*/)
	{
		return true;
	}

};


class SawWaveVoice : public SynthesiserVoice
{
public:
	SawWaveVoice() : level(0), modulo(0), inc(0)
	{
	}

	bool canPlaySound(SynthesiserSound* sound) override
	{
		return dynamic_cast<SawWaveSound*> (sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int currentPitchWheelPosition) override
	{
		level = velocity * 0.8;
		modulo = 0.0;
		trigger = 0;
		tailOff = 0.0;
		double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		inc = cyclesPerSecond / getSampleRate();
	}

	void stopNote(float, bool allowTailOff) override
	{
		if (allowTailOff)
		{
			if (tailOff == 0.0)
				tailOff = 1.0;
		}
		else
		{
			clearCurrentNote();
			modulo = 0.0;
		}
	}

	void pitchWheelMoved(int newValue) override
	{
		double normalised = (newValue - 8192) / 8192;
		/*	int nn = MidiMessage::getNoteNumber();
			startNote(nn,3, , normalised);*/
	}

	void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override
	{
		// not interested in controllers in this case.
	}

	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		processBlock(outputBuffer, startSample, numSamples);
	}

	void renderNextBlock(AudioBuffer<double>& outputBuffer, int startSample, int numSamples) override
	{
		processBlock(outputBuffer, startSample, numSamples);
	}

private:
	template <typename FloatType>
	void processBlock(AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples)
	{

		if (tailOff > 0.0)
		{
			if (modulo >= 1.0)
			{
				modulo -= 1.0;
				const FloatType currentSample = static_cast<FloatType> (trigger * (modulo * 2.0 - 1.0)) * level;

				for (int i = outputBuffer.getNumChannels(); --i >= 0;)
					outputBuffer.addSample(i, startSample, currentSample);


				modulo += inc;
				++startSample;
			}
			else
			{
				const FloatType currentSample = static_cast<FloatType> (trigger * (modulo * 2.0 - 1.0)) * level;

				for (int i = outputBuffer.getNumChannels(); --i >= 0;)
					outputBuffer.addSample(i, startSample, currentSample);

				modulo += inc;
				++startSample;

			}

			tailOff *= 0.99;

			if (tailOff <= 0.005)
			{
				clearCurrentNote();
				modulo = 0.0;
				trigger = 0;
			}

		}
		else
		{

			while (--numSamples >= 0)
			{
				if (modulo >= 1.0)
				{
					modulo -= 1.0;
					const FloatType currentSample = static_cast<FloatType> (modulo * 2.0 - 1.0) * level;

					for (int i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);


					modulo += inc;
					++startSample;
				}
				else
				{
					const FloatType currentSample =
						static_cast<FloatType> (modulo * 2.0 - 1.0) * level;

					for (int i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);

					modulo += inc;
					++startSample;

				}
			}
		}
	}
	double level, modulo, inc, tailOff;
	int trigger;
};

#endif