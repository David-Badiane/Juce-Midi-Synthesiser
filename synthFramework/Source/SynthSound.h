/*
  ==============================================================================

    SynthSound.h
    Created:
    Author:

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SynthSound : public SynthesiserSound
{
  public:
    bool appliesToNote (int /*midiNoteNumber*/)
    {
        return true;
    }
    
    bool appliesToChannel(int /*midiNoteNumber*/)
    {
        return true;
    }
};
