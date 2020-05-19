
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

		tailOff = 0.0;
		double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		inc = cyclesPerSecond / getSampleRate();
	}

	void stopNote(float, bool allowTailOff) override
	{
		adsr.noteOff();
	}

	void pitchWheelMoved(int newValue) override
	{
	
	}

	void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override
	{
		// not interested in controllers in this case.
	}

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
	double level, modulo, inc, tailOff;
	int trigger;
	ADSR adsr;
	ADSR::Parameters adsrParameters;
};
