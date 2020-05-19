#ifndef SINEWAVE_H
#define SINEWAVE_H

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



	void startNote(int midiNoteNumber,float velocity, SynthesiserSound*, int) override 
	{
		adsr.noteOn();
		currentAngle = 0.0;
		level = velocity * 0.3;
		tailOff = 0.0;
		double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		double cyclesPerSample = cyclesPerSecond / getSampleRate();
		angleDelta = cyclesPerSample * 2.0 * double_Pi;
	}

	void stopNote(float velocity, bool allowTailOff) override 
	{
		adsr.noteOff();
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
		adsr.setParameters(adsrParameters);
		processBlock(outputBuffer, startSample, numSamples);
	}

	
private:

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

	double currentAngle, angleDelta, level, tailOff;
	ADSR adsr;
	ADSR::Parameters adsrParameters;
};

#endif 