/*
  ==============================================================================

	Pedro Gerardo

	27.02.19 / Simple Filters

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VolumeKnob1AudioProcessorEditor::VolumeKnob1AudioProcessorEditor(VolumeKnob1AudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
{
	hpf_Value = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treestate, "hpf", hpf_slider);
	lowMid_Value = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treestate, "lmf", lowMid_Slider);
	highMid_Value = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treestate, "hmf", highMid_Slider);
	lpf_Value = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treestate, "lpf", lpf_Slider);


	//backgroundImage = ImageCache::getFromMemory(BinaryData::background_jpg, BinaryData::background_jpgSize);

	//========================HPF================================

	//Sliders
	hpf_slider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	hpf_slider.setColour(Slider::thumbColourId, Colours::orange);
	hpf_slider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	hpf_slider.setTextValueSuffix(" Hz");
	//hpf_slider.setRange(20, 140, 10);
	addAndMakeVisible(&hpf_slider);

	//Label
	hpf_Label.attachToComponent(&hpf_slider, false);
	hpf_Label.setText("HPF", dontSendNotification);
	hpf_Label.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(&hpf_Label);

	//========================LM Band============================

	//Slider
	lowMid_Slider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	lowMid_Slider.setColour(Slider::thumbColourId, Colours::mediumpurple);
	lowMid_Slider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	lowMid_Slider.setTextValueSuffix(" dB");
	//lowMid_Slider.setRange(-6.0, 6.0, 0.5);
	//lowMid_Slider.setValue(0);
	addAndMakeVisible(&lowMid_Slider);

	//Label
	lowMid_Label.attachToComponent(&lowMid_Slider, false);
	lowMid_Label.setText("Low Mid", dontSendNotification);
	lowMid_Label.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(&lowMid_Label);

	//========================HM Band============================

	//Slider
	highMid_Slider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	highMid_Slider.setColour(Slider::thumbColourId, Colours::mediumpurple);
	highMid_Slider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	highMid_Slider.setTextValueSuffix(" dB");
	//highMid_Slider.setRange(-6, 6, 0.5);
	//highMid_Slider.setValue(0);
	addAndMakeVisible(&highMid_Slider);

	//Label

	highMid_Label.attachToComponent(&highMid_Slider, false);
	highMid_Label.setText("High Mid", dontSendNotification);
	highMid_Label.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(&highMid_Slider);

	//=======================LPF=================================

	//Slider
	lpf_Slider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	lpf_Slider.setColour(Slider::thumbColourId, Colours::orange);
	lpf_Slider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	lpf_Slider.setTextValueSuffix(" KHz");
	//lpf_Slider.setRange(14, 20, 1);
	//lpf_Slider.setValue(20);
	addAndMakeVisible(&lpf_Slider);

	//Label

	lpf_Label.attachToComponent(&lpf_Slider, false);
	lpf_Label.setText("LPF", dontSendNotification);
	lpf_Label.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(&lpf_Slider);

	//background image
	background = ImageCache::getFromMemory(BinaryData::bg_duskEq_png, BinaryData::bg_duskEq_pngSize);

	//===========================================================
	setSize(600, 250);
	setResizable(true, true);

}

VolumeKnob1AudioProcessorEditor::~VolumeKnob1AudioProcessorEditor()
{
}

//==============================================================================
void VolumeKnob1AudioProcessorEditor::paint(Graphics& g)
{

	//g.fillAll(Colours::black);
	g.drawImageAt(background, 0, 0);

	/*g.setColour(Colours::orange);
	g.drawLine(30,220,570,220, 2.0f);*/

	auto outerLine = getLocalBounds().toFloat().reduced(10.0f);
	g.setColour(Colours::orange);
	g.drawRoundedRectangle(outerLine, 5.0f, 3.0f);


	auto innerLine = getLocalBounds().toFloat().reduced(20.0f);
	g.setColour(Colours::mediumpurple);
	g.drawRoundedRectangle(innerLine, 5.0f, 3.0f);


	g.setColour(Colours::orange);
	g.setFont(20.0f);
	g.drawFittedText("-Analog Filters-", getLocalBounds().reduced(30), Justification::centredBottom, 1);





}

//==============================================================================
void VolumeKnob1AudioProcessorEditor::resized()
{

	Rectangle<int> area = getLocalBounds();
	Rectangle<int> topArea = area.removeFromTop(50);
	Rectangle<int> bottomArea = area.removeFromBottom(50);


	Rectangle<int> filter1area = area.removeFromLeft(area.getWidth() / 4);

	hpf_slider.setBounds(filter1area.reduced(10));

	Rectangle<int> filter2area = area.removeFromLeft(area.getWidth() / 3);
	lowMid_Slider.setBounds(filter2area.reduced(20));


	Rectangle<int> filter3area = area.removeFromLeft(area.getWidth() / 2);
	highMid_Slider.setBounds(filter3area.reduced(20));


	Rectangle<int> filter4area = area.removeFromLeft(area.getWidth());
	lpf_Slider.setBounds(filter4area.reduced(10));


}
