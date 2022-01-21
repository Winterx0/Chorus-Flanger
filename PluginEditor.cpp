/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChorusFlangerAudioProcessorEditor::ChorusFlangerAudioProcessorEditor (ChorusFlangerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    auto& params = processor.getParameters();
//=====
        AudioParameterFloat* mDryWetParameter = (AudioParameterFloat*)params.getUnchecked(0);
            
        mDryWetSlider.setBounds(0, 0, 100, 100);
        mDryWetSlider.setSliderStyle(Slider::RotaryVerticalDrag);
        mDryWetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        mDryWetSlider.setRange(mDryWetParameter->range.start, mDryWetParameter->range.end);
        mDryWetSlider.setValue(*mDryWetParameter);
        addAndMakeVisible(mDryWetSlider);
            
        mDryWetSlider.onValueChange = [this, mDryWetParameter] {*mDryWetParameter = mDryWetSlider.getValue(); };
        mDryWetSlider.onDragStart = [mDryWetParameter] {mDryWetParameter->beginChangeGesture(); };
        mDryWetSlider.onDragEnd = [mDryWetParameter] {mDryWetParameter->endChangeGesture(); };
//====
        AudioParameterFloat* mDepthParameter = (AudioParameterFloat*)params.getUnchecked(1);
            
        mDepthSlider.setBounds(100, 0, 100, 100);
        mDepthSlider.setSliderStyle(Slider::RotaryVerticalDrag);
        mDepthSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        mDepthSlider.setRange(mDepthParameter->range.start, mDepthParameter->range.end);
        mDepthSlider.setValue(*mDepthParameter);
        addAndMakeVisible(mDepthSlider);
            
        mDepthSlider.onValueChange = [this, mDepthParameter] {*mDepthParameter = mDepthSlider.getValue(); };
        mDepthSlider.onDragStart = [mDepthParameter] {mDepthParameter->beginChangeGesture(); };
        mDepthSlider.onDragEnd = [mDepthParameter] {mDepthParameter->endChangeGesture(); };
        
//====
        AudioParameterFloat* mRateParameter = (AudioParameterFloat*)params.getUnchecked(2);
            
        mRateSlider.setBounds(200, 0, 100, 100);
        mRateSlider.setSliderStyle(Slider::RotaryVerticalDrag);
        mRateSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        mRateSlider.setRange(mRateParameter->range.start, mRateParameter->range.end);
        mRateSlider.setValue(*mRateParameter);
        addAndMakeVisible(mRateSlider);
            
        mRateSlider.onValueChange = [this, mRateParameter] {*mRateParameter = mRateSlider.getValue(); };
        mRateSlider.onDragStart = [mRateParameter] {mRateParameter->beginChangeGesture(); };
        mRateSlider.onDragEnd = [mRateParameter] {mRateParameter->endChangeGesture(); };
//====
        AudioParameterFloat* mPhaseOffSetParameter = (AudioParameterFloat*)params.getUnchecked(3);
            
        mPhaseOffSetSlider.setBounds(300, 0, 100, 100);
        mPhaseOffSetSlider.setSliderStyle(Slider::RotaryVerticalDrag);
        mPhaseOffSetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        mPhaseOffSetSlider.setRange(mPhaseOffSetParameter->range.start, mPhaseOffSetParameter->range.end);
        mPhaseOffSetSlider.setValue(*mPhaseOffSetParameter);
        addAndMakeVisible(mPhaseOffSetSlider);
            
        mPhaseOffSetSlider.onValueChange = [this, mPhaseOffSetParameter] {*mPhaseOffSetParameter = mRateSlider.getValue(); };
        mPhaseOffSetSlider.onDragStart = [mPhaseOffSetParameter] {mPhaseOffSetParameter->beginChangeGesture(); };
        mPhaseOffSetSlider.onDragEnd = [mPhaseOffSetParameter] {mPhaseOffSetParameter->endChangeGesture(); };
//====
        AudioParameterFloat* mFeedBackParameter = (AudioParameterFloat*)params.getUnchecked(4);
            
        mFeedBackSlider.setBounds(0, 100, 100, 100);
        mFeedBackSlider.setSliderStyle(Slider::RotaryVerticalDrag);
        mFeedBackSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        mFeedBackSlider.setRange(mFeedBackParameter->range.start, mFeedBackParameter->range.end);
        mFeedBackSlider.setValue(*mFeedBackParameter);
        addAndMakeVisible(mFeedBackSlider);
            
        mFeedBackSlider.onValueChange = [this, mFeedBackParameter] {*mFeedBackParameter = mFeedBackSlider.getValue(); };
        mFeedBackSlider.onDragStart = [mFeedBackParameter] {mFeedBackParameter->beginChangeGesture(); };
        mFeedBackSlider.onDragEnd = [mFeedBackParameter] {mFeedBackParameter->endChangeGesture(); };
//====
        AudioParameterInt* typeParameter = (AudioParameterInt*)params.getUnchecked(5);
        mType.setBounds(10, 260, 90, 30);
        mType.addItem("Chorus", 1);
        mType.addItem("Flanger", 2);
        addAndMakeVisible(mType);
        
        mType.onChange = [this, typeParameter]
        {
            typeParameter->beginChangeGesture();
            *typeParameter = mType.getSelectedItemIndex();
            typeParameter->endChangeGesture();
        };
        
    mType.setSelectedItemIndex(*typeParameter);
}

ChorusFlangerAudioProcessorEditor::~ChorusFlangerAudioProcessorEditor()
{
}

//==============================================================================
void ChorusFlangerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Bubbles", getLocalBounds(), Justification::centred, 1);
}

void ChorusFlangerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
