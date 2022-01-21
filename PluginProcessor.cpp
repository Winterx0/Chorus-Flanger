/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChorusFlangerAudioProcessor::ChorusFlangerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    /*Construct & Add Parameters*/
    addParameter(mDryWetParameter = new AudioParameterFloat("drywet",
                                                            "Dry Wet",
                                                            0.01,
                                                            1.0,
                                                            0.5));
    
    addParameter(mDepthParameter = new AudioParameterFloat("depth",
                                                            "Depth",
                                                            0.01,
                                                            1.0,
                                                            0.05));
    
    addParameter(mRateParameter = new AudioParameterFloat("rate",
                                                            "Rate",
                                                            0.01f,
                                                            20.f,
                                                            10.f));
    
    addParameter(mPhaseOffSetParameter = new AudioParameterFloat("phaseoffset",
                                                            "Phase Offset",
                                                            0.0f,
                                                            1.0f,
                                                            0.5f));
    
    addParameter(mFeedBackParameter = new AudioParameterFloat("feedback",
                                                              "Feedback",
                                                              0,
                                                              0.98,
                                                              0.5));
    
    addParameter(mTypeParameter = new AudioParameterInt("type",
                                                          "Type",
                                                          0,
                                                          1,
                                                          0));
    /*Initialize Data to Default Values*/
    mCircularBufferLeft = nullptr;
    mCircularBufferRight = nullptr;
    mCircularBufferWriteHead = 0;
    mCircularBufferLength = 0;
    
    mFeedBackLeft = 0;
    mFeedBackRight = 0;
    
    mLFOPhase = 0;
}

ChorusFlangerAudioProcessor::~ChorusFlangerAudioProcessor()
{
    if(mCircularBufferLeft != nullptr)
    {
        delete [] mCircularBufferLeft;
        mCircularBufferLeft = nullptr;
    }
    
    if(mCircularBufferRight != nullptr)
    {
        delete [] mCircularBufferRight;
        mCircularBufferRight = nullptr;
    }
}

//==============================================================================
const String ChorusFlangerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ChorusFlangerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ChorusFlangerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ChorusFlangerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ChorusFlangerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ChorusFlangerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ChorusFlangerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ChorusFlangerAudioProcessor::setCurrentProgram (int index)
{
}

const String ChorusFlangerAudioProcessor::getProgramName (int index)
{
    return {};
}

void ChorusFlangerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ChorusFlangerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    /*initialize data for current sample rate; reset Phase and writeHeads*/
    
    /*initialize phase*/
    mLFOPhase = 0;
     
    /*calculate circular buffer length*/
    mCircularBufferLength = sampleRate * MAX_DELAY_TIME;
     
    /*initialize left buffer*/
    if(mCircularBufferLeft == nullptr)
    {
        mCircularBufferLeft = new float[mCircularBufferLength];
    }
    /*clear junk data in new buffer*/
    zeromem(mCircularBufferLeft, mCircularBufferLength * sizeof(float));
     
    /*initialize right buffer*/
    if(mCircularBufferRight == nullptr)
    {
        mCircularBufferRight = new float[mCircularBufferLength];
    }
    /*clear junk data in new buffer*/
    zeromem(mCircularBufferRight, mCircularBufferLength * sizeof(float));
     
    /*initialize writeHear to 0*/
    mCircularBufferWriteHead = 0;
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void ChorusFlangerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ChorusFlangerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
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

void ChorusFlangerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    DBG("DRY WET:" << *mDryWetParameter);
    DBG("DEPTH:" << *mDepthParameter);
    DBG("RATE:" << *mRateParameter);
    DBG("FEEDBACK:" << *mFeedBackParameter);
    DBG("PHASE:" << *mPhaseOffSetParameter);
    DBG("TYPE:" << (int)*mTypeParameter);

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    /*obtain left and right audio data pointers*/
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);
    
    /*iterate all samples through the buffer*/
    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        /*write into circular buffer*/
        mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i] + mFeedBackLeft;
        mCircularBufferRight[mCircularBufferWriteHead] = rightChannel[i] + mFeedBackRight;
        
        /*generate LFO output*/
        float lfoOutLeft = sin(2*M_PI * mLFOPhase);
        
        /*Calculate Right Channel LFO Phase*/
        float lfoPhaseRight = mLFOPhase + *mPhaseOffSetParameter;
        
        if(lfoPhaseRight > 1)
        {
            lfoPhaseRight -= 1;
        }
        
        /*generate right channel LFO output*/
        float lfoOutRight = sin(2*M_PI * lfoPhaseRight);
        
        /*moves LFO phase forwad*/
        mLFOPhase += *mRateParameter / getSampleRate();
        if(mLFOPhase > 1)
        {
            mLFOPhase -= 1;
        }
        
        /*control parameter for LFO depth*/
        lfoOutLeft *= *mDepthParameter;
        lfoOutRight *= *mDepthParameter;
        
        /*map LFO output to desired delay times*/
        float lfoOutMappedLeft = 0;
        float lfoOutMappedRight = 0;
        
        if(mTypeParameter == 0) //Chorus
        {
            lfoOutMappedLeft = jmap(lfoOutLeft, -1.f, 1.f, 0.005f, 0.03f);
            lfoOutMappedRight = jmap(lfoOutRight, -1.f, 1.f, 0.005f, 0.03f);
        }
        else //Flanger
        {
            lfoOutMappedLeft = jmap(lfoOutLeft, -1.f, 1.f, 0.001f, 0.005f);
            lfoOutMappedRight = jmap(lfoOutRight, -1.f, 1.f, 0.001f, 0.005f);
        }
        
        /*calculate delay times in samples*/
        float delayTimeInSamplesLeft = getSampleRate() * lfoOutMappedLeft;
        float delayTimeInSamplesRight = getSampleRate() * lfoOutMappedRight;
        
        /*calulate left read head position*/
        float delayReadHeadLeft = mCircularBufferWriteHead - delayTimeInSamplesLeft;
        
        if(delayReadHeadLeft < 0)
        {
            delayReadHeadLeft += mCircularBufferLength;
        }
        
        /*calulate right read head position*/
        float delayReadHeadRight = mCircularBufferWriteHead - delayTimeInSamplesRight;
        
        if(delayReadHeadRight < 0)
        {
            delayReadHeadRight += mCircularBufferLength;
        }
        
        /*calculate interpolation points for left channel*/
        int readHeadLeft_x = (int)delayReadHeadLeft;
        int readHeadLeft_x1 = readHeadLeft_x + 1;
        float readHeadFloatLeft = delayReadHeadLeft - readHeadLeft_x;
            
        if(readHeadLeft_x1 >= mCircularBufferLength)
        {
            readHeadLeft_x1 -= mCircularBufferLength;
        }
        
        /*calculate interpolation points for right channel*/
        int readHeadRight_x = (int)delayReadHeadRight;
        int readHeadRight_x1 = readHeadRight_x + 1;
        float readHeadFloatRight = delayReadHeadRight - readHeadRight_x;
            
        if(readHeadRight_x1 >= mCircularBufferLength)
        {
            readHeadRight_x1 -= mCircularBufferLength;
        }
        
        /*generate left and right output samples*/
        float delay_sample_left = lin_interp(mCircularBufferLeft[readHeadLeft_x], mCircularBufferLeft[readHeadLeft_x1], readHeadFloatLeft);
        float delay_sample_right = lin_interp(mCircularBufferRight[readHeadRight_x], mCircularBufferRight[readHeadRight_x1], readHeadFloatRight);
        
        mFeedBackLeft = delay_sample_left * *mFeedBackParameter;
        mFeedBackRight = delay_sample_right * *mFeedBackParameter;
        
        mCircularBufferWriteHead++;
    
        if(mCircularBufferWriteHead >= mCircularBufferLength)
        {
            mCircularBufferWriteHead = 0;
        }
        
        float dryAmount = 1 - *mDryWetParameter;
        float wetAmount = *mDryWetParameter;
        
        buffer.setSample(0, i, buffer.getSample(0, i) * dryAmount + delay_sample_left * wetAmount);
        buffer.setSample(1, i, buffer.getSample(1, i) * dryAmount + delay_sample_right * wetAmount);
    }
}

//==============================================================================
bool ChorusFlangerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ChorusFlangerAudioProcessor::createEditor()
{
    return new ChorusFlangerAudioProcessorEditor (*this);
}

//==============================================================================
void ChorusFlangerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    std::unique_ptr<XmlElement> xml(new XmlElement("ChorusFlanger"));
    xml->setAttribute("DryWet", *mDryWetParameter);
    xml->setAttribute("Depth", *mDepthParameter);
    xml->setAttribute("Rate", *mRateParameter);
    xml->setAttribute("Feedback", *mFeedBackParameter);
    xml->setAttribute("PhaseOffSet", *mPhaseOffSetParameter);
    xml->setAttribute("Type", *mTypeParameter);
    
    copyXmlToBinary(*xml, destData);
     
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ChorusFlangerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if(xml.get() != nullptr && xml->hasTagName("ChorusFlanger"))
    {
        *mDryWetParameter = xml->getDoubleAttribute("DryWet");
        *mDepthParameter = xml->getDoubleAttribute("Depth");
        *mRateParameter = xml->getDoubleAttribute("Rate");
        *mFeedBackParameter = xml->getDoubleAttribute("Feedback");
        *mPhaseOffSetParameter = xml->getDoubleAttribute("PhaseOffSet");
        
        *mTypeParameter = xml->getIntAttribute("Type");
    }
    
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChorusFlangerAudioProcessor();
}

float ChorusFlangerAudioProcessor::lin_interp(float sample_x,float sample_x1, float inPhase)
{
    return (1 - inPhase) * sample_x + inPhase * sample_x1;
}
