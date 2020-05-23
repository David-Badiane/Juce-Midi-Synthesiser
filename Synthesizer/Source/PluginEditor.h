#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Oscillator.h"
#include "Envelope.h"
#include "Filter.h"


class SynthesizerAudioProcessorEditor : public AudioProcessorEditor,
                                        public Slider::Listener,
                                        public Button::Listener
{
public:
    SynthesizerAudioProcessorEditor(SynthesizerAudioProcessor&);
    ~SynthesizerAudioProcessorEditor();

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;

    float steps = 0.0f;



private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthesizerAudioProcessor& processor;

    Oscillator oscGui;
    Envelope envGui;
    Filter filterGui;
    Slider masterVol;
    Slider bendExtension;
    Slider pitchWheel;
    Slider modWheel;
    TextButton toggleFX;

    Slider::Listener* slider;

    void sliderValueChanged(Slider* slider) override;
    void buttonClicked(Button* button) override;

    
    
    MidiKeyboardComponent keyboardComponent;    
    TextEditor midiMessagesBox;
    
    double lfoFreq = 20;

    Timer* timerPointer;
  
    double startTime;
    
    Image logo;


    
    /*
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message);

    void addMessageToList(const MidiMessage& message, const String& source);
    void postMessageToList(const MidiMessage& message, const String& source);
    */
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthesizerAudioProcessorEditor);
};
