
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class Filter : public Component
{
public:
    Filter(SynthesizerAudioProcessor&);
    ~Filter();

    void paint(Graphics&) override;
    void resized() override;

private:
    Slider filterCutoff;
    Slider filterRes;

    ComboBox filterMenu;
    ComboBox filterKind;

    ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> filterKindVal;
    ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> resVal;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthesizerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Filter)
};