/*
  ==============================================================================

	Pedro Gerardo

	27.02.19 / Simple Filters

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VolumeKnob1AudioProcessor::VolumeKnob1AudioProcessor()
	: AudioProcessor(BusesProperties()
		.withInput("Input", AudioChannelSet::stereo(), true)
		.withOutput("Output", AudioChannelSet::stereo(), true)),
	treestate(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

//==============================================================================
VolumeKnob1AudioProcessor::~VolumeKnob1AudioProcessor()
{
}
AudioProcessorValueTreeState::ParameterLayout VolumeKnob1AudioProcessor::createParameterLayout()
{
	std::vector <std::unique_ptr<RangedAudioParameter>> params;

	auto hpfparams = std::make_unique<AudioParameterFloat>(HPF_ID, HPF_NAME, 12.0f, 220.0f, 0.0f);
	auto lowmidparams = std::make_unique<AudioParameterFloat>(LOW_MID_ID, LOW_MID_NAME, -12.0f, 12.0f, 0.0f);
	auto highmidparams = std::make_unique<AudioParameterFloat>(HIGH_MID_ID, HIGH_MID_NAME, -12.0f, 12.0f, 0.0f);
	auto lpfparams = std::make_unique<AudioParameterFloat>(LPF_ID, HPF_NAME, 2500.0f, 20000.0f, 20000.0f);

	params.push_back(std::move(hpfparams));
	params.push_back(std::move(lowmidparams));
	params.push_back(std::move(highmidparams));
	params.push_back(std::move(lpfparams));

	return { params.begin(),params.end() };
}

//==============================================================================
const String VolumeKnob1AudioProcessor::getName() const
{
	return JucePlugin_Name;
}
//==============================================================================
bool VolumeKnob1AudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}
//==============================================================================
bool VolumeKnob1AudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}
//==============================================================================
bool VolumeKnob1AudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}
//==============================================================================
double VolumeKnob1AudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}
//==============================================================================
int VolumeKnob1AudioProcessor::getNumPrograms()
{
	return 1;
}
//==============================================================================
int VolumeKnob1AudioProcessor::getCurrentProgram()
{
	return 0;
}
//==============================================================================
void VolumeKnob1AudioProcessor::setCurrentProgram(int index)
{
}
//==============================================================================
const String VolumeKnob1AudioProcessor::getProgramName(int index)
{
	return {};
}
//==============================================================================
void VolumeKnob1AudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void VolumeKnob1AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	lastSamplerate = sampleRate;

	dsp::ProcessSpec hpf, lmf, hmf, lpf;


	//HighPass
	hpf.sampleRate = sampleRate;
	hpf.maximumBlockSize = samplesPerBlock;
	hpf.numChannels = getTotalNumOutputChannels();

	highPassFilter.prepare(hpf);
	highPassFilter.reset();

	//Low Mid Frequency
	lmf.sampleRate = sampleRate;
	lmf.maximumBlockSize = samplesPerBlock;
	lmf.numChannels = getTotalNumOutputChannels();

	lowMidFrequency.prepare(lmf);
	lowMidFrequency.reset();

	//High Mid Frequency
	hmf.sampleRate = sampleRate;
	hmf.maximumBlockSize = samplesPerBlock;
	hmf.numChannels = getTotalNumOutputChannels();

	highMidFrequency.prepare(lmf);
	highMidFrequency.reset();

	//LowPassFilter
	lpf.sampleRate = sampleRate;
	lpf.maximumBlockSize = samplesPerBlock;
	lpf.numChannels = getTotalNumOutputChannels();

	lowPassFilter.prepare(lpf);
	lowPassFilter.reset();

	updateFilter();

}
//==============================================================================
void VolumeKnob1AudioProcessor::releaseResources()
{

}

//==============================================================================
#ifndef JucePlugin_PreferredChannelConfigurations
bool VolumeKnob1AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
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
void VolumeKnob1AudioProcessor::updateFilter()
{
	auto hpf_freq = *treestate.getRawParameterValue(HPF_ID);
	auto lmf_freq = *treestate.getRawParameterValue(LOW_MID_ID);
	auto hmf_freq = *treestate.getRawParameterValue(HIGH_MID_ID);
	auto lpf_freq = *treestate.getRawParameterValue(LPF_ID);

	*highPassFilter.state = *dsp::IIR::Coefficients<float>::makeHighPass(lastSamplerate, hpf_freq);
	*lowMidFrequency.state = *dsp::IIR::Coefficients<float>::makePeakFilter(lastSamplerate, 400.0f, 0.8f, Decibels::decibelsToGain(lmf_freq));
	*highMidFrequency.state = *dsp::IIR::Coefficients<float>::makePeakFilter(lastSamplerate, 2000.0f, 0.8f, Decibels::decibelsToGain(hmf_freq));
	*lowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(lastSamplerate, lpf_freq);


}

//==============================================================================

void VolumeKnob1AudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{

	ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	//Filters

	dsp::AudioBlock<float> block(buffer);
	updateFilter();
	highPassFilter.process(dsp::ProcessContextReplacing<float>(block));
	lowMidFrequency.process(dsp::ProcessContextReplacing<float>(block));
	highMidFrequency.process(dsp::ProcessContextReplacing<float>(block));
	lowPassFilter.process(dsp::ProcessContextReplacing<float>(block));

}
//==============================================================================
bool VolumeKnob1AudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}
//==============================================================================
AudioProcessorEditor* VolumeKnob1AudioProcessor::createEditor()
{
	return new VolumeKnob1AudioProcessorEditor(*this);
}

//==============================================================================
void VolumeKnob1AudioProcessor::getStateInformation(MemoryBlock& destData)
{
	auto state = treestate.copyState();
	std::unique_ptr<XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}
//==============================================================================
void VolumeKnob1AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(treestate.state.getType()))
			treestate.replaceState(ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new VolumeKnob1AudioProcessor();
}
