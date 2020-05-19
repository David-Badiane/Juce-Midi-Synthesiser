#include "PluginProcessor.h"
#include "PluginEditor.h"




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
        { std::make_unique<AudioParameterFloat>("attack", "Attack", NormalisableRange<float>(0.1f, 5.0f), 0.8f),
            std::make_unique<AudioParameterFloat>("decay", "Decay", NormalisableRange<float>(1.0f, 2.0f), 1.0f),
            std::make_unique<AudioParameterFloat>("sustain", "Sustain", NormalisableRange<float>(0.0f, 1.0f), 0.8f),
            std::make_unique<AudioParameterFloat>("release", "Release", NormalisableRange<float>(0.0001f, 8.0f), 0.01f),
            std::make_unique<AudioParameterFloat>("filterType", "FilterType", NormalisableRange<float>(0.0f, 2.0f), 0.0f),
            std::make_unique<AudioParameterFloat>("filterKind", "FilterKind", NormalisableRange<float>(0.0f, 2.0f), 0.0f),
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
    cutoff = 400.0;
    masterVolume = 1;
    oscBoxSelected = 1;
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

    switch (oscBoxSelected)
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

    case 5:
        for (int i = numVoices; --i >= 0;)
            mySynth.addVoice(new BleepSawWaveVoice());

        mySynth.addSound(new BleepSawWaveSound());

        break;

    case 6:
        for (int i = numVoices; --i >= 0;)
            mySynth.addVoice(new BleepSquareWaveVoice());

        mySynth.addSound(new BleepSquareWaveSound());

        break;

    case 7:
        for (int i = numVoices; --i >= 0;)
            mySynth.addVoice(new BleepTriangleWaveVoice());

        mySynth.addSound(new BleepTriangleWaveSound());

        break;

    case 8:
        for (int i = numVoices; --i >= 0;)
            mySynth.addVoice(new WhiteNoiseWaveVoice());

        mySynth.addSound(new WhiteNoiseWaveSound());

        break;

    case 9:
        for (int i = numVoices; --i >= 0;)
            mySynth.addVoice(new SineBeatsWaveVoice());

        mySynth.addSound(new SineBeatsWaveSound());

        break;

    case 10:
        for (int i = numVoices; --i >= 0;)
            mySynth.addVoice(new SineRissetBeatsWaveVoice());

        mySynth.addSound(new SineRissetBeatsWaveSound());

        break;
    
    case 11:
        for (int i = numVoices; --i >= 0;)
            mySynth.addVoice(new OrganWaveVoice());

        mySynth.addSound(new OrganWaveSound());

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

    float* attack = (float*)tree.getRawParameterValue("attack");
    float* release = (float*)tree.getRawParameterValue("release");
    float* sustain = (float*)tree.getRawParameterValue("sustain");
    float* decay = (float*)tree.getRawParameterValue("decay");
    

    for (int i = 0; i < mySynth.getNumVoices(); i++)
    {
        mySine                 -> setMasterVolume(masterVolume);
        mySquare               -> setMasterVolume(masterVolume);
        mySaw                  -> setMasterVolume(masterVolume);
        myTriangle             -> setMasterVolume(masterVolume);
                              
        myBleepSquare          -> setMasterVolume(masterVolume);
        myBleepSaw             -> setMasterVolume(masterVolume);
        myBleepTriangle        -> setMasterVolume(masterVolume);
        myWhiteNoise           -> setMasterVolume(masterVolume);
                             
        mySineBeats            -> setMasterVolume(masterVolume);
        myRissetBeats          -> setMasterVolume(masterVolume);
        myOrgan                -> setMasterVolume(masterVolume);

        switch (oscBoxSelected)
        {
        case 1:
            if ((mySine = dynamic_cast<SineWaveVoice*>(mySynth.getVoice(i))))
                mySine->setADSRParameters(attack, release, sustain, decay);
                
            break;

        case 2:
            if ((mySaw = dynamic_cast<SawWaveVoice*>(mySynth.getVoice(i))))
                mySaw->setADSRParameters(attack, release, sustain, decay);
            
            break;

        case 3:
            if ((mySquare = dynamic_cast<SquareWaveVoice*>(mySynth.getVoice(i))))
                mySquare->setADSRParameters(attack, release, sustain, decay);

            break;

        case 4:
            if ((myTriangle = dynamic_cast<TriangleWaveVoice*>(mySynth.getVoice(i))))
                myTriangle->setADSRParameters(attack, release, sustain, decay);

            break;

        case 5:
            if ((myBleepSaw = dynamic_cast<BleepSawWaveVoice*>(mySynth.getVoice(i))))
                myBleepSaw->setADSRParameters(attack, release, sustain, decay);

            break;

        case 6:
            if ((myBleepSquare = dynamic_cast<BleepSquareWaveVoice*>(mySynth.getVoice(i))))
                myBleepSquare->setADSRParameters(attack, release, sustain, decay);

            break;

        case 7:
            if ((myBleepTriangle = dynamic_cast<BleepTriangleWaveVoice*>(mySynth.getVoice(i))))
                myBleepTriangle->setADSRParameters(attack, release, sustain, decay);
            
            break;

        case 8:
            if ((myWhiteNoise = dynamic_cast<WhiteNoiseWaveVoice*>(mySynth.getVoice(i))))
               myWhiteNoise->setADSRParameters(attack, release, sustain, decay);

            break;

        case 9:
            if ((mySineBeats = dynamic_cast<SineBeatsWaveVoice*>(mySynth.getVoice(i))))
            {   
                mySineBeats->update_beats(deltaFrequency);
                mySineBeats->setADSRParameters(attack, release, sustain, decay);
            }
            break;

        case 10:
            if ((myRissetBeats = dynamic_cast<SineRissetBeatsWaveVoice*>(mySynth.getVoice(i))))
            {
                myRissetBeats->update_beats(deltaFrequency);
                myRissetBeats->setADSRParameters(attack, release, sustain, decay);
            }
            break;

        case 11:
            if ((myOrgan = dynamic_cast<OrganWaveVoice*>(mySynth.getVoice(i))))
            {
                myOrgan->update_beats(deltaFrequency);
                myOrgan->setADSRParameters(attack, release, sustain, decay);
            }

            break;
        }

    }


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