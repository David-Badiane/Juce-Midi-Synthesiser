
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "../JuceLibraryCode/BinaryData.h"
#include "myLookAndFeel.h"

//==============================================================================
/*
*/
class Filter : public Component,
                      public Slider::Listener
{
public:
    Filter(SynthesizerAudioProcessor&);
    ~Filter();

    void paint(Graphics&) override;
    void resized() override;

private:
    Slider filterCutoff;
    Slider filterRes;

    Slider::Listener* slider;

    ComboBox filterMenu;
    ComboBox filterKind;

    void sliderValueChanged(Slider *slider) override;

    ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> filterKindVal;
    ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> resVal;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthesizerAudioProcessor& processor;
    myLookAndFeelV1 myLookAndFeelV1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Filter)
};