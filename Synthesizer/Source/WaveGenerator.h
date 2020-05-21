

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
	WaveGeneratorVoice() : masterGain(1), inc(0), modWheel(0),
						   noteFrequency(0), wheelCoordinate(0)
	{
	}

	bool canPlaySound(SynthesiserSound* sound) override
	{
		return dynamic_cast<WaveGeneratorSound*> (sound) != nullptr;
	}

//========================================================================================

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

//========================================================================================

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int) override
	{
		
	}

	void stopNote(float velocity, bool allowTailOff) override
	{
		adsr.noteOff();
	}

//========================================================================================

	void pitchWheelMoved(int value)
	{
		setPitchBend(value);
	}

	void roughValuePitchWheelMove(int value) //rough value pass
	{
		wheelCoordinate = value;
	}

	void setBendExtension(int extension)
	{
		pitchBendUpSemitones = extension;
		pitchBendDownSemitones = -extension;
	}
	
	void recalculatePitch() {
		inc = noteFrequency * std::pow(2.0, pitchBendCents() / 1200) / getSampleRate();
	}

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

//========================================================================================

	static double noteHz(int midiNoteNumber, double centsOffset)
	{
		double hertz = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		hertz *= std::pow(2.0, centsOffset / 1200);
		return hertz;
	}

//========================================================================================

	void setModWheel(double pos) 
	{
    	modWheel = pos;
    	stretchFrequencies();
    }
	
	void stretchFrequencies() {
		int sign = 1;
		if (noteFrequency > 10000) {
			decrescentmod = true;
		}
		else if (noteFrequency < 40) {
			decrescentmod = false;
		}
		if (decrescentmod) {
			sign = -1;
		}

		noteFrequency *= std::pow(2.0, 64 * modWheel * sign / 1200); //change pitchBendCents() with modwheel
		inc = noteFrequency / getSampleRate();
	}

//========================================================================================


	void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override
	{
	}

//========================================================================================

    
	void setMasterVolume(double volz) {
		masterGain = volz;
	}

//========================================================================================

	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
	}


protected:

	double masterGain;
	double wheelCoordinate;
	double modWheel;
	double inc;

	bool decrescentmod = false;
	ADSR adsr;
	ADSR::Parameters adsrParameters;

	int pitchBendUpSemitones = 2;
	int pitchBendDownSemitones = -2;
	double noteFrequency;
};
