#include <JuceHeader.h>
#include "WaveGenerator.h"
//==============================================================================
/*
*/
class SineRissetBeatsWaveSound : public WaveGeneratorSound {
};

class SineRissetBeatsWaveVoice : public WaveGeneratorVoice {

public:
	SineRissetBeatsWaveVoice() : currentAngle{ 0,0,0,0,0,0,0,0 }, angleDelta{ 0,0,0,0,0,0,0,0 }, 
		                         currentAngle2{ 0,0,0,0,0,0,0,0 }, angleDelta2{ 0,0,0,0,0,0,0,0 },
								 level(0), deltaFreq(0) 
								 {}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int) override
	{
		adsr.noteOn();
		noteFrequency = noteHz(midiNoteNumber, pitchBendCents());
		for (int i = 0; i < 8; i++)
		{
			currentAngle[i] = 0;
			currentAngle2[i] = 0;
			double frequency = noteFrequency + i * deltaFreq;
			inc = frequency / getSampleRate();
			angleDelta[i] = inc * 2.0 * double_Pi;

			frequency = noteFrequency + i * 0.9 * deltaFreq;
			inc = frequency / getSampleRate();
			angleDelta2[i] = inc * 2.0 * double_Pi;
		}

		level = velocity * 0.1;
	}

	void recalculatePitch() {
		double freq = noteFrequency * std::pow(2.0, pitchBendCents() / 1200);
		for (int i = 0; i < 8; i++)
		{
			double frequency = freq + i * deltaFreq;
			inc = frequency / getSampleRate();
			angleDelta[i] = inc * 2.0 * double_Pi;

			frequency = freq + i * 0.7 * deltaFreq;
			inc = frequency / getSampleRate();
			angleDelta2[i] = inc * 2.0 * double_Pi;
		}
	}

	void update_beats(double deltaFreqParam)
	{
		deltaFreq = deltaFreqParam/4;
		for (int i = 0; i < 8; i++)
		{
			double frequency = noteFrequency + i * deltaFreq;
			double cyclesPerSample = frequency / getSampleRate();
			angleDelta[i] = cyclesPerSample * 2.0 * double_Pi;

			frequency = noteFrequency + i * 0.9 * deltaFreq;
			cyclesPerSample = frequency / getSampleRate();
			angleDelta2[i] = cyclesPerSample * 2.0 * double_Pi;
		}
	}

	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		adsr.setSampleRate(getSampleRate());
		adsr.setParameters(adsrParameters);
		processBlock(outputBuffer, startSample, numSamples);
	}



private:



	void processBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
	{

		while (--numSamples >= 0)
		{
			float Sample1 = 0;
			float Sample2 = 0;
			for (int i = 0; i < 8; i++)
			{
				Sample1 = Sample1 + (float)(std::sin(currentAngle[i] + 2 * double_Pi * modWheel) * level * masterGain);
				Sample2 = Sample2 + (float)(std::sin(currentAngle2[i] + 2 * double_Pi * modWheel) * level * masterGain);
			}
		
			outputBuffer.addSample(0, startSample, adsr.getNextSample() * Sample1);
			outputBuffer.addSample(1, startSample, adsr.getNextSample() * Sample2);

			for (int i = 0; i < 8; i++)
			{
				currentAngle[i] += angleDelta[i];
				currentAngle2[i] += angleDelta2[i];
			}
			++startSample;
		}
	}

	double currentAngle[8], angleDelta[8], level, deltaFreq, currentAngle2[8], angleDelta2[8];
};