#ifndef SINEWAVE_H
#define SINEWAVE_H

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../JuceLibraryCode/JuceHeader.h"


class SineWaveSound : public SynthesiserSound 
{
public:
	SineWaveSound() {}//constructor

	bool appliesToNote(int /*midi note nmbr*/) override
	{ return true; }
	
	bool appliesToChannel(int /*midi channel nmbr*/)  override
	{ return true; }
};

class SineWaveVoice : public SynthesiserVoice
{
public:
	SineWaveVoice() : currentAngle(0), angleDelta(0), level(0)
	{
	}

	bool canPlaySound(SynthesiserSound* sound) override 
	{ 
		return dynamic_cast<SineWaveSound*> (sound) != nullptr;
	}																	
																		
	void startNote(int midiNoteNumber,float velocity, SynthesiserSound*, int) override 
	{
		currentAngle = 0.0;
		level = velocity * 0.8;
		tailOff = 0.0;
		double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		double cyclesPerSample = cyclesPerSecond / getSampleRate();

		angleDelta = cyclesPerSample * 2.0 * double_Pi;
	}

	void stopNote(float velocity, bool allowTailOff) override 
	{
		if (allowTailOff)
		{
			if (tailOff == 0.0)
				tailOff = 1.0;
		}
		else
		{
			clearCurrentNote();
			angleDelta = 0.0;
		}
	}

	void pitchWheelMoved(int /*newValue*/) override
	{
		// can't be bothered implementing this for the demo!
	}

	void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override
	{
		// not interested in controllers in this case.
	}

	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		processBlock(outputBuffer, startSample, numSamples);
	}

	
private:

	template <typename FloatType>
	void processBlock(AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples)
	{
		if (angleDelta != 0.0)
		{
			if (tailOff > 0.0) 
			{
				while (--numSamples >= 0)
				{
					auto currentSample = (float)(std::sin(currentAngle) * level * tailOff);

					for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);

					currentAngle += angleDelta;
					++startSample;

					tailOff *= 0.99;

					if (tailOff <= 0.005)
					{
						clearCurrentNote(); 

						angleDelta = 0.0;
						break;
					}
				}
			}
			else
			{
				while (--numSamples >= 0) 
				{
					auto currentSample = (float)(std::sin(currentAngle) * level);

					for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);

					currentAngle += angleDelta;
					++startSample;
				}
			}
		}
	}

	double currentAngle, angleDelta, level, tailOff;
};

#endif 