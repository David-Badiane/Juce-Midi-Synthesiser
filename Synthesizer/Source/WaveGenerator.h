

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
	WaveGeneratorVoice() : masterGain(1)
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

	void pitchWheelMoved(int value) override
	{
		setPitchBend(value);
		
	}

	void roughValuePitchWheelMove(int value) //rough value pass
	{
		wheelCoordinate = value;
	}

	void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override
	{
		// not interested in controllers in this case.
	}

	void setMasterVolume(double volz) {
		masterGain = volz;
	}

	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
	}
		
	double masterGain;
	double wheelCoordinate;

	void setPitchBend(int pitchWheelPos)
	{
		if (pitchWheelPos >= 0)
		{
			// shifting up
			wheelCoordinate = float(pitchWheelPos) / 64;
		}
		else
		{
			// shifting down
			wheelCoordinate = float(pitchWheelPos) / 64;    // negative number
		}
	}



	float pitchBendCents()
	{
		if (wheelCoordinate >= 0.0f)
		{
			// shifting up
			return wheelCoordinate * pitchBendUpSemitones * 100;
		}
		else
		{
			// shifting down
			return -1 * wheelCoordinate * pitchBendDownSemitones * 100;
		}
	}


	static double noteHz(int midiNoteNumber, double centsOffset)
	{
		double hertz = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		hertz *= std::pow(2.0, centsOffset / 1200);
		return hertz;
	}


protected:

	ADSR adsr;
	ADSR::Parameters adsrParameters;

	int pitchBendUpSemitones = 2;
	int pitchBendDownSemitones = -2;
	double noteFrequency;
	
};
