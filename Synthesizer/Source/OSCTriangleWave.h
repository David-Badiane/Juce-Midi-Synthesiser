#ifndef TRIANGLEWAVE_H_INCLUDED
#define TRIANGLEWAVE_H_INCLUDED


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

	void setADSRSampleRate(double sampleRate)
	{
		adsr.setSampleRate(sampleRate);
	}

	void setADSRParameters(float* att,
		float* dec,
		float* sus,
		float* rel)
	{
		adsrParameters.attack = *att;
		adsrParameters.decay  = *dec;
		adsrParameters.sustain = *sus;
		adsrParameters.release = *rel ;
	}


	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int currentPitchWheelPosition) override
	{
		adsr.noteOn();
		level = velocity * 0.3;
		modulo = 0.0;
		trigger = 0;
		tailOff = 0;
		currentState = 1.0;
		double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		inc = cyclesPerSecond / getSampleRate();

	}

	void stopNote(float velocity, bool allowTailOff) override
	{
		adsr.noteOff();
	}

	void pitchWheelMoved(int) override {}
	void controllerMoved(int, int) override {}

	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		adsr.setParameters(adsrParameters);
		processBlock(outputBuffer, startSample, numSamples);
	}

	

private:
	template <typename FloatType>
	void processBlock(AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples)
	{
		while (--numSamples >= 0)
		{
			if (modulo >= 1)
			{
				modulo = 0.0;
				currentState = 2.0 * abs(2.0 * modulo - 1.0) - 1.0;
				const FloatType currentSample = static_cast<FloatType> (currentState * level);
				for (int i = outputBuffer.getNumChannels(); --i >= 0;) {
					outputBuffer.addSample(i, startSample, adsr.getNextSample() * currentSample);
				}

				modulo += inc;
				++startSample;
			}
			else
			{
				currentState = 2.0 * abs(2.0 * modulo - 1.0) - 1.0;
				const FloatType currentSample = static_cast<FloatType> (currentState * level);
				for (int i = outputBuffer.getNumChannels(); --i >= 0;) {
					outputBuffer.addSample(i, startSample, adsr.getNextSample() * currentSample);
				}

				modulo += inc;
				++startSample;
			}
		}
	
	}

	double level, inc, modulo, currentState, tailOff;
	int trigger;
	ADSR adsr;
	ADSR::Parameters adsrParameters;
};

#endif 