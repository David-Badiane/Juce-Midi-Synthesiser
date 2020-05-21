#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SineWave.h"
#include "SawWave.h"
#include "SquareWave.h"
#include "TriangleWave.h"
#include "BleepSawWave.h"
#include "BleepSquareWave.h"
#include "BleepTriangleWave.h"
#include "WhiteNoiseWave.h"
#include "SineBeatsWave.h"
#include "SineRissetBeatsWave.h"
#include "OrganWave.h"


//==============================================================================

class SynthesizerAudioProcessor : public AudioProcessor,
                                  public MidiKeyboardStateListener
{
public:
    //==============================================================================
    SynthesizerAudioProcessor();
    ~SynthesizerAudioProcessor();

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String& newName) override;

    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================

    AudioProcessorValueTreeState tree;

    void updateFilter();
    void process(dsp::ProcessContextReplacing<float> context);

    int oscBoxSelected;
    double deltaFrequency;
    const int numVoices = 3;

    int bendExt;
    void initialiseSynth();
    double cutoff;
    double masterVolume;
    double pitchWheel;
    double modWheel;
    MidiKeyboardState keyboardState;


private:
    Synthesiser mySynth;

    SineWaveVoice* mySine;
    SquareWaveVoice* mySquare;
    SawWaveVoice* mySaw;
    TriangleWaveVoice* myTriangle;
    
    BleepSquareWaveVoice* myBleepSquare;
    BleepSawWaveVoice* myBleepSaw;
    BleepTriangleWaveVoice* myBleepTriangle;
    WhiteNoiseWaveVoice* myWhiteNoise;

    SineBeatsWaveVoice* mySineBeats;
    SineRissetBeatsWaveVoice* myRissetBeats;
    OrganWaveVoice* myOrgan;

    AudioProcessorEditor* editor = getActiveEditor();


    void handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handlePitchWheel(MidiKeyboardState*, int midiChannel, int wheelValue);
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message);

    void setMidiInput(int index);
    int lastInputIndex;
    bool isAddingFromMidiInput;



    // this is a processor duplicator of the filter, not a filter, we got The Filter itself and the Parameters inside the State Variable filter, pointers
    dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter <float>, dsp::StateVariableFilter::Parameters <float>> stateVariableFilter;
    dsp::ProcessorDuplicator<dsp::IIR::Filter <float>, dsp::IIR::Coefficients <float>> IIRFilter;
    dsp::ProcessorDuplicator<dsp::FIR::Filter <float>, dsp::FIR::Coefficients <float>> FIRFilter;

    // There is no implementation for lowpass into the Coefficients for FIR filters, it's elsewhere
    // FIR implementation are inside dsp::FilterDesign, for FIR filters we need some windowing methods also

    double lastSampleRate;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthesizerAudioProcessor)
};