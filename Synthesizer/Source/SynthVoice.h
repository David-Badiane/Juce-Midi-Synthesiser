
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "maximilian.h"

class SynthVoice : public SynthesiserVoice
{
public:
    bool canPlaySound(SynthesiserSound* sound) override
    {
        return dynamic_cast <SynthSound*>(sound) != nullptr;
    }

    void setPitchBend(int pitchWheelPos)
    {
        if (pitchWheelPos > 8192)
        {
            // shifting up
            pitchBend = float(pitchWheelPos - 8192) / (16383 - 8192);
        }
        else
        {
            // shifting down
            pitchBend = float(8192 - pitchWheelPos) / -8192;    // negative number
        }
    }

    float pitchBendCents()
    {
        if (pitchBend >= 0.0f)
        {
            // shifting up
            return pitchBend * pitchBendUpSemitones * 100;
        }
        else
        {
            // shifting down
            return pitchBend * pitchBendDownSemitones * 100;
        }
    }

    static double noteHz(int midiNoteNumber, double centsOffset)
    {
        double hertz = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        hertz *= std::pow(2.0, centsOffset / 1200);
        return hertz;
    }

    //=======================================================

    void getOscType(std::atomic<float>* selection)
    {
        theWave = *selection;

    }

    //=======================================================
    void  getSampleRate(double currentSampleRate)
    {
        sampleRate = currentSampleRate;
    }

    //=======================================================

    double setOscType()

    {
        double sample1;

        if (theWave == 0)
        {
            sample1 = osc1.sinewave(frequency * 0.5);
        }
        if (theWave == 1)
        {
            sample1 = osc1.saw(frequency);
        }
        if (theWave == 2)
        {
            sample1 = osc1.sinewave(frequency * 2);
        }
        if (theWave == 3)
        {
            sample1 = osc1.triangle(frequency * 1.5);
        }

        return sample1;
    }

    //=======================================================

    void getEnvelopeParams(std::atomic<float>* attack,
                           std::atomic<float>* decay, 
                           std::atomic<float>* sustain, 
                           std::atomic<float>* release)
    {
        env1.setAttack(*attack);
        env1.setDecay(*decay);
        env1.setSustain(*sustain);
        env1.setRelease(*release);
    }

    //=======================================================

    double setEnvelope()
    {
        return env1.adsr(setOscType(), env1.trigger);
    }

    //=======================================================

    void getWillsParams(std::atomic<float>* mGain, 
                        std::atomic<float>* blend, 
                        std::atomic<float>* pbup, 
                        std::atomic<float>* pbdn)
    {
        masterGain = *mGain;
        osc2blend = *blend;
        pitchBendUpSemitones = *pbup;
        pitchBendDownSemitones = *pbdn;
    }

    void getFilterParams(std::atomic<float>* filterType,
                         std::atomic<float>* filterCutoff,
                         std::atomic<float>* filterRes)
    {
        filterChoice = *filterType;
        cutoff = *filterCutoff;
        resonance = *filterRes;
    }

    

    //=======================================================

    void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override
    {
        noteNumber = midiNoteNumber;
        env1.trigger = 1;
        setPitchBend(currentPitchWheelPosition);
        frequency = noteHz(noteNumber, pitchBendCents());
        level = velocity;
    }

    //=======================================================

    void stopNote(float velocity, bool allowTailOff) override
    {
        env1.trigger = 0;
        allowTailOff = true;

        if (velocity == 0)
            clearCurrentNote();
    }

    //=======================================================

    void pitchWheelMoved(int newPitchWheelValue) override
    {
        setPitchBend(newPitchWheelValue);
        frequency = noteHz(noteNumber, pitchBendCents());
        //increment = frequency * wtSize / lastSampleRate;

        
    }

    //=======================================================

    void controllerMoved(int controllerNumber, int newControllerValue) override
    {

    }

    //=======================================================

    void renderNextBlock(AudioBuffer <float>& outputBuffer, int startSample, int numSamples) override
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                outputBuffer.addSample(channel, startSample, setEnvelope() * masterGain);
                //phase = fmod ((phase + increment), wtSize);
            }
            ++startSample;
        }
    }

    //=======================================================
private:
    double level;
    double frequency;
    int theWave, theWave2;

    float masterGain;
    float osc2blend;

    int noteNumber;
    float pitchBend = 0.0f;
    float pitchBendUpSemitones = 2.0f;
    float pitchBendDownSemitones = 2.0f;

    int filterChoice;
    float cutoff;
    float resonance;

    maxiOsc osc1, osc2;
    maxiEnv env1;
    maxiFilter filter1;

    double sampleRate;

};