/*
  ==============================================================================

    SynthVoice.h
    Created: 
    Author:

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "maximilian.h"


class SynthVoice : public SynthesiserVoice
{
    
public:
    
    bool canPlaySound (SynthesiserSound* sound) override
    {
       return dynamic_cast<SynthSound*>(sound) != nullptr;   
    }
    
    //=========================================//
    
    void getEnvelopeParams (float* attack, float* release)
    {
        env1.setAttack(double(*attack));
        env1.setRelease(double(*release));
    }
    
    //=========================================//
    
    void startNote(int midiNoteNumber, float velocity, SynthesiserSound*sound, int currentPitchWheelPosition) override
    {
        env1.trigger = 1;
        level = velocity;
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    }
    
    //=========================================//
    
    void stopNote (float velocity, bool allowTailOff) override
    {
        env1.trigger = 0;
        allowTailOff = true;
        
        if (velocity == 0)
            clearCurrentNote();
    }
    
    //=========================================//
     
    void pitchWheelMoved (int newPitchWheelValue) override
    {
        
    }
    
    //=========================================//
    
    void controllerMoved (int controllerNumber, int newControllerValue) override
    {
        
    }
    
    //=========================================//
    
    void renderNextBlock (AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override
    {
        
        env1.setDecay(500);
        env1.setSustain(0.8);
        
        for (int sample = 0; sample < numSamples; ++sample)
        {
            double theWave = osc1.saw(frequency);
            double theSound = env1.adsr(theWave, env1.trigger) * level;
            double filteredSound = filter1.lores(theSound, 40, 0.1); //(Sound,cutOff,Resonance)
            
            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                outputBuffer.addSample(channel, startSample, filteredSound);
            }
            
            ++startSample;
        }
    
    }
    
    //=========================================//
    
private :
    double level;
    double frequency;
    
    maxiOsc osc1;
    maxiEnv env1;
    maxiFilter filter1;
};
