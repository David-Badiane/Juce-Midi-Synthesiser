#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class Envelope    : public Component
{
public:
    Envelope(SynthesizerAudioProcessor&);
    ~Envelope();

    void paint (Graphics&) override;
    void resized() override;


private:
    
    Slider attackSlider;
    Slider sustainSlider;
    Slider decaySlider;
    Slider releaseSlider;

   // void sliderValueChanged(Slider* slider) override;

    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> attackVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> decayVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> sustainVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> releaseVal;


    // This reference is provided as a quick way for your editor
    // to access the processor object that created it 
    SynthesizerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Envelope)
};
