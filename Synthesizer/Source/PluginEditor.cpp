
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthesizerAudioProcessorEditor::SynthesizerAudioProcessorEditor(SynthesizerAudioProcessor& p)
    :AudioProcessorEditor(&p), processor(p), oscGui(p), envGui(p), filterGui(p)
{
    setSize(1000, 200);

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

}


