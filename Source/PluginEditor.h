/*
  ==============================================================================

	Pedro Gerardo

	27.02.19 / Simple Filters

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class VolumeKnob1AudioProcessorEditor  : public AudioProcessorEditor
{
public:
    VolumeKnob1AudioProcessorEditor (VolumeKnob1AudioProcessor&);
    ~VolumeKnob1AudioProcessorEditor();

	
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;


    VolumeKnob1AudioProcessor& processor;
	//==============================================================================

	// Sliders for Frequency controls
	Slider hpf_slider;
	Slider lowMid_Slider;
	Slider highMid_Slider;
	Slider lpf_Slider;
	
	//==============================================================================

	//Labels for the rectangles
	Label hpf_Label;
	Label lowMid_Label;
	Label highMid_Label;
	Label lpf_Label;
	
	Image background;
	
	
private:

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> hpf_Value;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> lowMid_Value;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> highMid_Value;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> lpf_Value;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VolumeKnob1AudioProcessorEditor)
};
