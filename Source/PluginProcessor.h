/*
  ==============================================================================

	Pedro Gerardo

	27.02.19 / Simple Filters

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define HPF_ID "hpf"
#define HPF_NAME "HPF"
#define LOW_MID_ID "lmf"
#define LOW_MID_NAME "LMF"
#define HIGH_MID_ID "hmf"
#define HIGH_MID_NAME "HMF"
#define LPF_ID "lpf"
#define LPF_NAME "LPF"

//==============================================================================
/**
*/
class VolumeKnob1AudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    VolumeKnob1AudioProcessor();
    ~VolumeKnob1AudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
	void updateFilter();

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
	
	
	AudioProcessorValueTreeState treestate;
	AudioProcessorValueTreeState::ParameterLayout createParameterLayout();


private:
	
	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> highPassFilter, lowMidFrequency, highMidFrequency, lowPassFilter;
	
	
	float lastSamplerate;
	

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VolumeKnob1AudioProcessor)
};
