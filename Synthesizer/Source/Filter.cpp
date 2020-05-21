#include "../JuceLibraryCode/JuceHeader.h"
#include "Filter.h"

//==============================================================================
Filter::Filter(SynthesizerAudioProcessor& p) :
    processor(p)
{
    setSize(200, 200);

    filterMenu.addItem("Low Pass ", 1);
    filterMenu.addItem("High Pass ", 2);
    filterMenu.addItem("Band Pass ", 3);
    
    filterKind.addItem("State Variable Filter", 1);
    filterKind.addItem("IIR", 2);
    filterKind.addItem("FIR", 3);

    filterMenu.setJustificationType(Justification::centred);
    addAndMakeVisible(&filterMenu);

    filterKind.setJustificationType(Justification::centred);
    addAndMakeVisible(&filterKind);
   
    filterCutoff.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filterCutoff.setLookAndFeel(&knobStyle);
    filterCutoff.setRange(20.0, 10000.0);
    filterCutoff.setValue(400.0);
    filterCutoff.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    filterCutoff.setPopupDisplayEnabled(true, true, this);
    filterCutoff.setSkewFactorFromMidPoint(400);
    addAndMakeVisible(&filterCutoff);

    filterRes.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filterRes.setLookAndFeel(&knobStyle);
    filterRes.setRange(1, 5);
    filterRes.setValue(1);
    filterRes.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    filterRes.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&filterRes);

    filterCutoff.addListener(this);

    filterTypeVal = new AudioProcessorValueTreeState::ComboBoxAttachment(processor.tree, "filterType", filterMenu);
    filterKindVal = new AudioProcessorValueTreeState::ComboBoxAttachment(processor.tree, "filterKind", filterKind);
    resVal = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "filterRes", filterRes);
}

Filter::~Filter()
{
    filterCutoff.setLookAndFeel(nullptr);
    filterRes.setLookAndFeel(nullptr);
}

void Filter::paint(Graphics& g)
{
    //background ui stuff
    juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);

    g.setColour(Colours::lightgrey);
    g.setFont(Font("Courier", 15.0f, Font::bold));
    g.drawText("Filter", titleArea, Justification::centredTop);

    juce::Rectangle <float> area(25, 33, 150, 150);

    g.setColour(Colours::darkgoldenrod);
    g.drawRoundedRectangle(area, 20.0f, 8.0f);
}

void Filter::resized()
{
    //need to come back and dynamically set these...ok for now
    juce::Rectangle<int> area = getLocalBounds().reduced(40);
    filterMenu.setBounds(area.removeFromTop(20));
    filterKind.setBounds(area.removeFromTop(40));
    filterCutoff.setBounds(30, 100, 70, 70);
    filterRes.setBounds(100, 100, 70, 70);
}

void Filter::sliderValueChanged(Slider* slider)
{
    processor.cutoff = filterCutoff.getValue();
}
