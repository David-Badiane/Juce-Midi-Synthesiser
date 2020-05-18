#ifndef TRIANGLEWAVE_H_INCLUDED
#define TRIANGLEWAVE_H_INCLUDED

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../JuceLibraryCode/JuceHeader.h"



class TriangleWaveSound : public SynthesiserSound
{
public:
	TriangleWaveSound() {}

	virtual bool appliesToNote(int /*midi note nmbr*/)
	{
		return true;
	}

	virtual bool appliesToChannel(int /*midi channel nmbr*/)
	{
		return true;
	}

};



class TriangleWaveVoice : public SynthesiserVoice
{
public:
	TriangleWaveVoice() : level(0), modulo(0), inc(0)
	{
	}

	bool canPlaySound(SynthesiserSound* sound)
	{
		return dynamic_cast<TriangleWaveSound*> (sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int currentPitchWheelPosition) override
	{
		level = velocity * 0.5;
		modulo = 0.0;
		trigger = 0;
		tailOff = 0;
		currentState = 1.0;
		double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		inc = cyclesPerSecond / getSampleRate();

	}

	void stopNote(float velocity, bool allowTailOff) override
	{
		if (allowTailOff)
		{
			if (tailOff == 0.0)
				tailOff = 0.5;
		}
		else
		{
			clearCurrentNote();
			modulo = 0.0;
		}
	}

	void pitchWheelMoved(int) override {}
	void controllerMoved(int, int) override {}

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

			if (modulo >= 1)
			{
				modulo = 0.0;
				currentState = 2.0 * abs(2.0 * modulo - 1.0) - 1.0;
				const FloatType currentSample = static_cast<FloatType> (trigger * (currentState * level));
				for (int i = outputBuffer.getNumChannels(); --i >= 0;) {
					outputBuffer.addSample(i, startSample, currentSample);
				}

				modulo += inc;
				++startSample;
			}
			else
			{
				currentState = 2.0 * abs(2.0 * modulo - 1.0) - 1.0;
				const FloatType currentSample = static_cast<FloatType> (trigger * (currentState * level));
				for (int i = outputBuffer.getNumChannels(); --i >= 0;) {
					outputBuffer.addSample(i, startSample, currentSample);
				}

				modulo += inc;
				++startSample;
			}

			tailOff *= 0.99;

			if (tailOff <= 0.005)
			{
				clearCurrentNote();
				trigger = 0;
				modulo = 0.0;
			}

		}
		else
		{
			while (--numSamples >= 0)
			{
				if (modulo >= 1)
				{
					modulo = 0.0;
					currentState = 2.0 * abs(2.0 * modulo - 1.0) - 1.0;
					const FloatType currentSample = static_cast<FloatType> (currentState * level);
					for (int i = outputBuffer.getNumChannels(); --i >= 0;) {
						outputBuffer.addSample(i, startSample, currentSample);
					}

					modulo += inc;
					++startSample;
				}
				else
				{
					currentState = 2.0 * abs(2.0 * modulo - 1.0) - 1.0;
					const FloatType currentSample = static_cast<FloatType> (currentState * level);
					for (int i = outputBuffer.getNumChannels(); --i >= 0;) {
						outputBuffer.addSample(i, startSample, currentSample);
					}

					modulo += inc;
					++startSample;
				}
			}
		}
	}

	double level, inc, modulo, currentState, tailOff;
	int trigger;
};

#endif 