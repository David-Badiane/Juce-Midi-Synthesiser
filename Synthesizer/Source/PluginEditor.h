#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Oscillator.h"
#include "Envelope.h"
#include "Filter.h"


class SynthesizerAudioProcessorEditor : public AudioProcessorEditor
{
public:
    SynthesizerAudioProcessorEditor(SynthesizerAudioProcessor&);
    ~SynthesizerAudioProcessorEditor();

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;



private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthesizerAudioProcessor& processor;

    Oscillator oscGui;
    Envelope envGui;
    Filter filterGui;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthesizerAudioProcessorEditor);
};