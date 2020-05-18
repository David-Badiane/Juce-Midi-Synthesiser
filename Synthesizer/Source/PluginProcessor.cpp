#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "OSCSineWave.h"
#include "OSCSawWave.h"
#include "OSCSquareWave.h"
#include "OSCTriangleWave.h"


//==============================================================================

SynthesizerAudioProcessor::SynthesizerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    :AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
        ),
    tree(*this, nullptr, "PARAMETERS",
        { std::make_unique<AudioParameterFloat>("attack", "Attack", NormalisableRange<float>(0.1f, 5000.0f), 0.1f),
            std::make_unique<AudioParameterFloat>("decay", "Decay", NormalisableRange<float>(1.0f, 2000.0f), 1.0f),
            std::make_unique<AudioParameterFloat>("sustain", "Sustain", NormalisableRange<float>(0.0f, 1.0f), 0.8f),
            std::make_unique<AudioParameterFloat>("release", "Release", NormalisableRange<float>(0.1f, 5000.0f), 0.1f),
            std::make_unique<AudioParameterFloat>("wavetype", "Wavetype", NormalisableRange<float>(0.0f, 3.0f), 0.0f),
            std::make_unique<AudioParameterFloat>("filterType", "FilterType", NormalisableRange<float>(0.0f, 2.0f), 0.0f),
            std::make_unique<AudioParameterFloat>("filterKind", "FilterKind", NormalisableRange<float>(0.0f, 2.0f), 0.0f),
            std::make_unique<AudioParameterFloat>("filterCutoff", "FilterCutoff", NormalisableRange<float>(20.0f, 10000.0f), 400.0f),
            std::make_unique<AudioParameterFloat>("filterRes", "FilterRes", NormalisableRange<float>(1.0f, 5.0f), 1.0f),
            std::make_unique<AudioParameterFloat>("blend", "Osc2Blend", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
            std::make_unique<AudioParameterFloat>("mastergain", "MasterGain", NormalisableRange<float>(0.0f, 1.0f), 1.0f),
            std::make_unique<AudioParameterFloat>("pbup", "PBup", NormalisableRange<float>(1.0f, 12.0f), 2.0f),
            std::make_unique<AudioParameterFloat>("pbdown", "PBdown", NormalisableRange<float>(1.0f, 12.0f), 2.0f),
        })

#endif

    //=======================================================================
    //initialisation
{   
    box_selected = 1;
    initialiseSynth();
}

SynthesizerAudioProcessor::~SynthesizerAudioProcessor()
{
}

//==============================================================================


void SynthesizerAudioProcessor::initialiseSynth()
{
    mySynth.clearVoices();
    mySynth.clearSounds();

    switch (box_selected)
    {
    case 1:
        for (int i = numVoices; --i >= 0;) {
            mySynth.addVoice(new SineWaveVoice());
        }

        mySynth.addSound(new SineWaveSound());

        break;

    case 2:
        for (int i = numVoices; --i >= 0;) {
            mySynth.addVoice(new SawWaveVoice());
        }

        mySynth.addSound(new SawWaveSound());

        break;

    case 3:
        for (int i = numVoices; --i >= 0;)
            mySynth.addVoice(new SquareWaveVoice());

        mySynth.addSound(new SquareWaveSound());

        break;

    case 4:
        for (int i = numVoices; --i >= 0;)
            mySynth.addVoice(new TriangleWaveVoice());

        mySynth.addSound(new TriangleWaveSound());

        break;
    }
}

//==============================================================================

const String SynthesizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthesizerAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SynthesizerAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SynthesizerAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double SynthesizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthesizerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynthesizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthesizerAudioProcessor::setCurrentProgram(int index)
{
}

const String SynthesizerAudioProcessor::getProgramName(int index)
{
    return {};
}

void SynthesizerAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================

void SynthesizerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    ignoreUnused(samplesPerBlock);
    lastSampleRate = sampleRate;
    mySynth.setCurrentPlaybackSampleRate(lastSampleRate);

    dsp::ProcessSpec spec;   // the spec class passes the sampleRate, num channels and maximumblock size
    spec.sampleRate = lastSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    stateVariableFilter.reset();
    stateVariableFilter.prepare(spec);     // prepares to play for  the things to pass to the state variable filter

    IIRFilter.reset();
    IIRFilter.prepare(spec);     // prepares to play for  the things to pass to the state variable filter

    FIRFilter.reset();
    FIRFilter.prepare(spec);

    updateFilter();
}

void SynthesizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void SynthesizerAudioProcessor::updateFilter()
{
    int menuChoice = *tree.getRawParameterValue("filterType");
    int cutoff = *tree.getRawParameterValue("filterCutoff");
    int res = *tree.getRawParameterValue("filterRes");

    if (menuChoice == 0)
    {
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, res);

        *IIRFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, cutoff, res);

        *FIRFilter.state = *dsp::FilterDesign<float>::designFIRLowpassWindowMethod(cutoff, lastSampleRate, 21, dsp::WindowingFunction <float>::hamming); // 21 is the filter order number 
    }
    if (menuChoice == 1)
    {
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, res);

        *IIRFilter.state = *dsp::IIR::Coefficients<float>::makeHighPass(lastSampleRate, cutoff, res);

        *FIRFilter.state = *dsp::FilterDesign<float>::designFIRLowpassWindowMethod(cutoff, lastSampleRate, 21, dsp::WindowingFunction <float>::hamming); // 21 is the filter order number 

    }
    if (menuChoice == 2)
    {
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, res);

        *IIRFilter.state = *dsp::IIR::Coefficients<float>::makeBandPass(lastSampleRate, cutoff, res);
    }
}


void SynthesizerAudioProcessor::process(dsp::ProcessContextReplacing<float> context)
{
    // DO THE PROCESSING HERE 
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthesizerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif



void SynthesizerAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    int filterKind = *tree.getRawParameterValue("filterKind");

    ScopedNoDenormals noDenormals;
    //const int totalNumInputChannels  = getTotalNumInputChannels();
    //const int totalNumOutputChannels = getTotalNumOutputChannels();

    buffer.clear();

    mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    updateFilter();
    dsp::AudioBlock<float> block(buffer); // applies the processing to the buffer
    switch (filterKind)
    {
    case 0:
        stateVariableFilter.process(dsp::ProcessContextReplacing<float>(block));   // and substitutes it into the context 
        break;
    case 1:
        IIRFilter.process(dsp::ProcessContextReplacing<float>(block));
        break;
    case 2:
        FIRFilter.process(dsp::ProcessContextReplacing<float>(block));
        break;
    }
}

//==============================================================================
bool SynthesizerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SynthesizerAudioProcessor::createEditor()
{
    return new SynthesizerAudioProcessorEditor(*this);
}

//==============================================================================
void SynthesizerAudioProcessor::getStateInformation(MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SynthesizerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthesizerAudioProcessor();
}