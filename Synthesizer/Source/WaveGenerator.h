

#pragma once
#include <JuceHeader.h>


class WaveGeneratorSound    : public SynthesiserSound 

{
public:
	WaveGeneratorSound() {}//constructor

	bool appliesToNote(int /*midi note nmbr*/) override
	{
		return true;
	}

	bool appliesToChannel(int /*midi channel nmbr*/)  override
	{
		return true;
	}
};

class WaveGeneratorVoice : public SynthesiserVoice
{
public:
	WaveGeneratorVoice() 
	{
	}

	bool canPlaySound(SynthesiserSound* sound) override
	{
		return dynamic_cast<WaveGeneratorSound*> (sound) != nullptr;
	}

	void setADSRSampleRate(double sampleRate)
	{
		adsr.setSampleRate(sampleRate);
	}

	void setADSRParameters(float*att,
		float*dec,
		float*sus,
		float*rel)
	{
		adsrParameters.attack = *att;
		adsrParameters.decay = *dec;
		adsrParameters.sustain = *sus;
		adsrParameters.release = *rel;
	}



	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int) override
	{
		
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
	}
		



protected:
	
	ADSR adsr;
	ADSR::Parameters adsrParameters;
};
