#include "Adsr.h"

double Adsr::nextSample()
{
	if (currentState != OFF && currentState != SUSTAIN)
	{

		if (currentSampleIndex == nextStateSampleIndex)
		{
			EnvelopeState newState = static_cast<EnvelopeState>((currentState + 1) % kNumEnvelopeStates);
			enterState(newState);
		}
		currentLevel *= multiplier;
		currentSampleIndex++;
	}
	return currentLevel;
}

void Adsr::enterState(EnvelopeState newState)
{
	currentState = newState;
	currentSampleIndex = 0;

	if (currentState == OFF || currentState == SUSTAIN)
	{
		nextStateSampleIndex = 0;
	}
	else
	{
		nextStateSampleIndex = stateValue[currentState] * sampleRate / 1000;
	}

	switch (newState)
	{
	case OFF:
		currentLevel = 0.0;
		multiplier = 1.0;
		break;
	case ATTACK:
		currentLevel = minimumLevel;
		calculateMultiplier(minimumLevel, 1.0, nextStateSampleIndex);
		break;
	case DECAY:
		currentLevel = 1.0;
		calculateMultiplier(currentLevel, fmax(stateValue[SUSTAIN], minimumLevel), nextStateSampleIndex);
		break;
	case SUSTAIN:
		currentLevel = stateValue[SUSTAIN];
		multiplier = 1.0;
		break;
	case RELASE:
		calculateMultiplier(currentLevel, minimumLevel, nextStateSampleIndex);
		break;
	default:
		break;
	}
}

void Adsr::calculateMultiplier(double startLevel, double endLevel, unsigned long long lengthInSamples)
{
	multiplier = 1.0 + (log(endLevel) - log(startLevel)) / (lengthInSamples);

}

void Adsr::setSampleRate(double newSampleRate)
{
	sampleRate = newSampleRate;
}