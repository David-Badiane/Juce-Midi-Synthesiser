
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthesizerAudioProcessorEditor::SynthesizerAudioProcessorEditor(SynthesizerAudioProcessor& p)
    :AudioProcessorEditor(&p), processor(p), oscGui(p), envGui(p), filterGui(p)
{
    setSize(1000, 200);
    setResizable(true, true);

    masterVol.setSliderStyle(Slider::LinearVertical);
    masterVol.setRange(0.0, 1, 0.01);
    masterVol.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    masterVol.setPopupDisplayEnabled(true, false, this);
    masterVol.setTextValueSuffix(" Volume");
    masterVol.setValue(1.0);


    addAndMakeVisible(&masterVol);
    addAndMakeVisible(&oscGui);
    addAndMakeVisible(&envGui);
    addAndMakeVisible(&filterGui);

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

    oscGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    filterGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    envGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    masterVol.setBounds(area.removeFromLeft(componentWidth / 3).removeFromBottom(componentHeight - 30).removeFromTop(componentHeight - 50));
}


void SynthesizerAudioProcessorEditor::sliderValueChanged(Slider* slider) {
    processor.masterVolume = masterVol.getValue();
}