
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class KnobLookAndFeel : public LookAndFeel_V4
{
public:
    KnobLookAndFeel()
    {
        setColour(Slider::thumbColourId, Colours::red);
    }




    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
        auto radius = jmin(width / 2, height / 2) - 10.0f;
        auto centreX = x + width * 0.5f;
        auto centreY = y + height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        // fill



        g.setColour(Colours::darkgoldenrod);
        g.fillEllipse(rx, ry, rw, rw);



        // outline
        g.setColour(Colours::black);
        g.drawEllipse(rx, ry, rw, rw, 4.0f);



        Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 2.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
        // pointer
        g.setColour(Colours::black);
        g.fillPath(p);
    }
};

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

    KnobLookAndFeel knobStyle;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Filter)
};