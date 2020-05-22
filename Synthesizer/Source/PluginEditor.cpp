
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthesizerAudioProcessorEditor::SynthesizerAudioProcessorEditor(SynthesizerAudioProcessor& p)
    :AudioProcessorEditor(&p), processor(p), oscGui(p), envGui(p), filterGui(p),
    keyboardComponent(p.keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    setSize(800, 300);
    setResizable(true, true);

    
    masterVol.setSliderStyle(Slider::LinearVertical);
    masterVol.setRange(0.0, 3, 0.01);
    masterVol.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    masterVol.setPopupDisplayEnabled(true, false, this);
    masterVol.setTextValueSuffix("Master Volume");
    masterVol.setSkewFactorFromMidPoint(0.5);
    masterVol.setValue(1.0);

    pitchWheel.setSliderStyle(Slider::LinearVertical);
    pitchWheel.setRange(-64, 64, 1);
    pitchWheel.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    pitchWheel.setValue(0.0);

    modWheel.setSliderStyle(Slider::LinearVertical);
    modWheel.setRange(0, 1, double(1 / 128));
    modWheel.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    modWheel.setValue(0.0);

    bendExtension.setSliderStyle(Slider::SliderStyle::LinearVertical);
    bendExtension.setRange(0, 12, 1);
    bendExtension.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    bendExtension.setTextValueSuffix("Bend Extension");
    bendExtension.setValue(0.0);
	
	keyboardComponent.setColour(keyboardComponent.blackNoteColourId, Colours::darkslategrey);
    keyboardComponent.setColour(keyboardComponent.whiteNoteColourId, Colours::lightgrey);

    masterVol.addListener(this);
    pitchWheel.addListener(this);
    modWheel.addListener(this);
    bendExtension.addListener(this);

    addAndMakeVisible(&bendExtension);
    addAndMakeVisible(&pitchWheel);
    addAndMakeVisible(&modWheel);
    addAndMakeVisible(&masterVol);
    addAndMakeVisible(&oscGui);
    addAndMakeVisible(&envGui);
    addAndMakeVisible(&filterGui);


    addAndMakeVisible(keyboardComponent);
    logo = ImageCache::getFromMemory(BinaryData::whatever_png, BinaryData::whatever_pngSize);
}

SynthesizerAudioProcessorEditor::~SynthesizerAudioProcessorEditor()
{
    setOpaque(false);
}





//==============================================================================
void SynthesizerAudioProcessorEditor::paint(Graphics& g)
{       
    g.drawImage(logo, 670,0,130,38,0,0,logo.getWidth(), logo.getHeight());
    auto baseColour = Colour(0x282C34);
    juce::Rectangle<float> area = getLocalBounds().toFloat();
    g.fillAll(baseColour.contrasting(((float)std::sin(lfoFreq / 1000 * steps) + (float)1.8) / (float)16));
    steps++;
    juce::Rectangle<int> titleArea1(0, 30, 60, 40);
    juce::Rectangle<int> titleArea2(9, 20, 40, 30);
    g.setColour(Colours::lightgrey);
    g.setFont(Font("Courier", 9.0f, Font::italic));
    g.drawText("BEND", titleArea2, Justification::centred);
    g.drawText("INTERVAL", titleArea1, Justification::centred);
    repaint();
   
}

void SynthesizerAudioProcessorEditor::resized()
{
    juce::Rectangle<int> area = getLocalBounds();

    const int componentWidth = 200;
    const int componentHeight = 200;

    masterVol.setColour(masterVol.thumbColourId, Colours::limegreen);
    masterVol.onDragStart = [this] {masterVol.setColour(masterVol.thumbColourId, Colours::darkgoldenrod); };
    masterVol.onDragEnd = [this] {masterVol.setColour(masterVol.thumbColourId, Colours::limegreen); };
    masterVol.setColour(masterVol.trackColourId, Colours::limegreen);

    pitchWheel.setColour(pitchWheel.thumbColourId, Colours::darkgoldenrod);
    pitchWheel.onDragStart = [this] { pitchWheel.setColour(pitchWheel.thumbColourId, Colours::yellow); };
    pitchWheel.onDragEnd = [this] { pitchWheel.setValue(0.0, dontSendNotification);
                                    processor.pitchWheel = 0;
                                    pitchWheel.setColour(pitchWheel.thumbColourId, Colours::darkgoldenrod);
    };

    modWheel.setColour(modWheel.thumbColourId, Colours::darkgoldenrod);
    modWheel.onDragStart = [this] { modWheel.setColour(modWheel.thumbColourId, Colours::yellow); };
    modWheel.onDragEnd = [this] { modWheel.setColour(modWheel.thumbColourId, Colours::darkgoldenrod);};

    bendExtension.setColour(bendExtension.thumbColourId, Colours::purple);
    bendExtension.setColour(bendExtension.trackColourId, Colours::purple);
    bendExtension.onDragStart = [this] { bendExtension.setColour(bendExtension.thumbColourId, Colours::black); };
    bendExtension.onDragEnd = [this] { bendExtension.setColour(bendExtension.thumbColourId, Colours::purple);};

    juce::Rectangle<int> extArea(20, 50, 20, 100);
    bendExtension.setBounds(extArea);
    pitchWheel.setBounds(area.removeFromLeft(componentWidth / 9).removeFromBottom(componentHeight / 2).removeFromTop(componentHeight - 50));
    modWheel.setBounds(area.removeFromLeft(componentWidth / 9).removeFromBottom(componentHeight / 2).removeFromTop(componentHeight - 50).removeFromRight(20));
    keyboardComponent.setBounds(area.removeFromBottom(componentHeight/2));
    oscGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    filterGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    envGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    masterVol.setBounds(area.removeFromRight(componentWidth / 3).removeFromBottom(componentHeight - 30).removeFromTop(componentHeight - 50));
    
}


void SynthesizerAudioProcessorEditor::sliderValueChanged(Slider* slider) {
    processor.masterVolume = masterVol.getValue();
    processor.pitchWheel = pitchWheel.getValue();
    processor.modWheel = modWheel.getValue();
    processor.bendExt = bendExtension.getValue();
}

