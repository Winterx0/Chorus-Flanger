/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ChorusFlangerAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    ChorusFlangerAudioProcessorEditor (ChorusFlangerAudioProcessor&);
    ~ChorusFlangerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    
    Slider mDryWetSlider;
    Slider mDepthSlider;
    Slider mRateSlider;
    Slider mPhaseOffSetSlider;
    Slider mFeedBackSlider;
    
    ComboBox mType;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ChorusFlangerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChorusFlangerAudioProcessorEditor)
};
