
#include "../JuceLibraryCode/JuceHeader.h"
#include "Oscillator.h"

//==============================================================================
Oscillator::Oscillator(SynthesizerAudioProcessor& p) :
    processor(p)
{
    setSize(200, 200);

    oscMenu.addItem("Sine Wave", 1);
    oscMenu.addItem("Saw Wave", 2);
    oscMenu.addItem("Square Wave", 3);
    oscMenu.addItem("Triangle Wave", 4);
    oscMenu.addItem("BleepSaw Wave", 5);
    oscMenu.addItem("BleepSquare Wave", 6);
    oscMenu.addItem("BleepTriangle Wave", 7);
    oscMenu.addItem("White Noise + Sin Wave", 8);
    oscMenu.addItem("Sine Beats Wave", 9);
    oscMenu.addItem("Sine Risset Beats Wave", 10);
    oscMenu.addItem("Organ Wave", 11);
    oscMenu.setJustificationType(Justification::centred);
    addAndMakeVisible(&oscMenu);

    numVoices.addItem("monophonic", 1);
    numVoices.addItem("2 voices", 2);
    numVoices.addItem("3 voices", 3);
    numVoices.addItem("4 voices", 4);
    numVoices.addItem("5 voices", 5);
    numVoices.setJustificationType(Justification::centred);
    addAndMakeVisible(&numVoices);


    numVoices.setSelectedId(3);
    numVoices.addListener(this);
    oscMenu.setSelectedId(1);
    oscMenu.addListener(this);

    deltaFreq.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    deltaFreq.setRange(0.1f, 20.0f);
    deltaFreq.setValue(10.0f);
    deltaFreq.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&deltaFreq);
    deltaFreq.addListener(this);
}

Oscillator::~Oscillator()
{
}

void Oscillator::paint(Graphics& g)
{
    //background stuff
    juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);
    g.setColour(Colours::white);
    g.drawText("Oscillators", titleArea, Justification::centredTop);
    juce::Rectangle <float> area(25, 25, 150, 150);
    g.setColour(Colours::red);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void Oscillator::resized()
{
    juce::Rectangle<int> area = getLocalBounds().reduced(40);
    oscMenu.setBounds(area.removeFromTop(20));
    numVoices.setBounds(area.removeFromTop(20));
    deltaFreq.setBounds(area.removeFromTop(50));
}

void Oscillator::comboBoxChanged(ComboBox* boxThatChanged)
{
    processor.oscBoxSelected = oscMenu.getSelectedId();
    processor.numVoices = numVoices.getSelectedId();
    processor.initialiseSynth();
}

void Oscillator::sliderValueChanged(Slider* slider) 
{
    processor.deltaFrequency = deltaFreq.getValue();
}