

#include "../JuceLibraryCode/JuceHeader.h"


class SquareWaveSound : public SynthesiserSound
{
public:
	SquareWaveSound() {}

	virtual bool appliesToNote(int /*midi note nmbr*/)
	{
		return true;
	}

	virtual bool appliesToChannel(int /*midi channel nmbr*/)
	{
		return true;
	}

};


class SquareWaveVoice : public SynthesiserVoice
{
public:
	SquareWaveVoice() : level(0), modulo(0), inc(0)
	{
	}

	bool canPlaySound(SynthesiserSound* sound) override
	{
		return dynamic_cast<SquareWaveSound*> (sound) != nullptr;
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
		adsrParameters.decay = *dec;
		adsrParameters.sustain = *sus;
		adsrParameters.release = *rel;
	}


	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int currentPitchWheelPosition) override
	{
		adsr.noteOn();
		level = velocity * 0.3;
		modulo = 0.0;
		tailOff = 0.0;
		trigger = 0;
		currentState = 1.0;

		double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber) * 2.0;
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
		while(--numSamples >= 0)
		{
			if (modulo >= 1)
			{
				modulo -= 1.0;
				currentState *= (-1.0);
				const FloatType currentSample = static_cast<FloatType> (currentState * level);
				for (int i = outputBuffer.getNumChannels(); --i >= 0;) {
					outputBuffer.addSample(i, startSample,  adsr.getNextSample() * currentSample);
				}

				modulo += inc;
				++startSample;
			}
			else
			{
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
