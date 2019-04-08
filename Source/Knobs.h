/*
  ==============================================================================

    Knobs.h
    Created: 8 Apr 2019 3:23:34pm
    Author:  Gerardo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class BasicLookAndFeel : public LookAndFeel_V4
{
public:
	void drawRotarySlider(Graphics& g, 
		int x, int y, 
		int width, int height, 
		float sliderPos,
		const float rotaryStartAngle, 
		const float rotaryEndAngle, 
		Slider& slider) override;
};