#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Oscillator.h"
#include "Envelope.h"
#include "Filter.h"


class SynthesizerAudioProcessorEditor : public AudioProcessorEditor,
                                        public Slider::Listener
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
    Slider masterVol;
    Slider pitchWheel;
    Slider modWheel;

    Slider::Listener* slider;
    void sliderValueChanged(Slider* slider) override;

    
    MidiKeyboardComponent keyboardComponent;    
    TextEditor midiMessagesBox;
    MidiKeyboardState keyboardState;

  
    double startTime;


    
    /*
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message);

    void addMessageToList(const MidiMessage& message, const String& source);
    void postMessageToList(const MidiMessage& message, const String& source);
    */
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthesizerAudioProcessorEditor);
};
