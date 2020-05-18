
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

    addAndMakeVisible(&oscBox);
    oscBox.addItem("sine OSC", 1);
    oscBox.addItem("saw OSC", 2);
    oscBox.addItem("square OSC", 3);
    oscBox.addItem("triangle OSC", 4);
    oscBox.setSelectedId(1);
    oscBox.addListener(this);
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

    auto sideBarArea = area.removeFromRight(jmax(80, area.getWidth() / 4));

    auto sideItemHeight = 40;
    auto sideItemMargin = 5;
    oscBox.setBounds(sideBarArea.removeFromTop(sideItemHeight).reduced(sideItemMargin));
}

void SynthesizerAudioProcessorEditor::comboBoxChanged(ComboBox* boxThatChanged)
{
    processor.box_selected = boxThatChanged->getSelectedId();
    processor.initialiseSynth();
}
