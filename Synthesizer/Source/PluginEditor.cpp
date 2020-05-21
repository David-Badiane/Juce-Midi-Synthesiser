
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthesizerAudioProcessorEditor::SynthesizerAudioProcessorEditor(SynthesizerAudioProcessor& p)
    :AudioProcessorEditor(&p), processor(p), oscGui(p), envGui(p), filterGui(p),
    keyboardComponent(p.keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    setSize(900, 300);
    setResizable(true, true);

    masterVol.setSliderStyle(Slider::LinearVertical);
    masterVol.setRange(0.0, 2, 0.01);
    masterVol.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    masterVol.setPopupDisplayEnabled(true, false, this);
    masterVol.setTextValueSuffix("Master Volume");
    masterVol.setValue(1.0);

    pitchWheel.setSliderStyle(Slider::LinearVertical);
    pitchWheel.setRange(-64, 64, 1);
    pitchWheel.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    pitchWheel.setValue(0.0);

    modWheel.setSliderStyle(Slider::LinearVertical);
    modWheel.setRange(0, 1, double(1 / 128));
    modWheel.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    modWheel.setValue(0.0);

    masterVol.addListener(this);
    pitchWheel.addListener(this);
    modWheel.addListener(this);

    addAndMakeVisible(&pitchWheel);
    addAndMakeVisible(&modWheel);
    addAndMakeVisible(&masterVol);
    addAndMakeVisible(&oscGui);
    addAndMakeVisible(&envGui);
    addAndMakeVisible(&filterGui);


    addAndMakeVisible(keyboardComponent);

}

SynthesizerAudioProcessorEditor::~SynthesizerAudioProcessorEditor()
{
}

//==============================================================================
void SynthesizerAudioProcessorEditor::paint(Graphics& g)
{
    g.fillAll(Colours::black);
}

void SynthesizerAudioProcessorEditor::resized()
{
    juce::Rectangle<int> area = getLocalBounds();

    const int componentWidth = 200;
    const int componentHeight = 200;

    pitchWheel.setBounds(area.removeFromLeft(componentWidth / 3).removeFromBottom(componentHeight / 2).removeFromTop(componentHeight - 50));
    modWheel.setBounds(area.removeFromLeft(componentWidth / 3).removeFromBottom(componentHeight / 2).removeFromTop(componentHeight - 50));
    pitchWheel.onDragEnd = [this] { pitchWheel.setValue(0.0, dontSendNotification); 
                                    processor.pitchWheel = 0;
                                   };
    keyboardComponent.setBounds(area.removeFromBottom(componentHeight/2));
    oscGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    filterGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    envGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    masterVol.setBounds(area.removeFromLeft(componentWidth / 3).removeFromBottom(componentHeight - 30).removeFromTop(componentHeight - 50));

    
}


void SynthesizerAudioProcessorEditor::sliderValueChanged(Slider* slider) {
    processor.masterVolume = masterVol.getValue();
    processor.pitchWheel = pitchWheel.getValue();
    processor.modWheel = modWheel.getValue();
}

