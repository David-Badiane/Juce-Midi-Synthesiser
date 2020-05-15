/*
  ==============================================================================

    SynthSound.h
    Created: 15 May 2020 2:09:22pm
    Author:  Francesco Lorenzo

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
