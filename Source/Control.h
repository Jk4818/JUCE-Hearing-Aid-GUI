

#pragma once

#include <JuceHeader.h>

namespace Gui {

    //Horizontal Appearance Class
    class HorizontalSliderLookAndFeel : public juce::LookAndFeel_V4{
    public:

        void drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider)  override {

            //Temporary Full Container Background
            //g.setColour(juce::Colours::darkblue);
            //g.fillRect(x,y,width,height);

            //Main Bar Styling
            auto barHeight = height / 6;

            auto centreX = (float)x + (float)width * 0.5f;
            auto centreY = (float)y + ((float)height - barHeight) * 0.5f;

            auto noIntervals = (slider.getMaximum() -slider.getMinimum())/ (slider.getInterval() * 2);
            auto intervalSize = width / noIntervals;
            //Slider interval line
            g.setColour(juce::Colour(0xFFFFFFFF));
            for (int i = 0; i <= noIntervals; i++) {
                g.drawVerticalLine(x + i * intervalSize, slider.getTextBoxHeight() + height / 4, slider.getTextBoxHeight() + height / 2);
            }

            //Draw main Bar
            juce::Rectangle<float> horizontalBar(x, centreY, width, barHeight);

            g.setColour(juce::Colour(0xFF4F6367));
            g.fillRect(horizontalBar);
            g.drawRoundedRectangle(horizontalBar, horizontalBar.getHeight() / 2, 4);

            //Slider inteval label
            auto textSize = 10.0f;
            g.setFont(juce::Font("Montserrat", textSize, juce::Font::bold));
            g.setColour(juce::Colour(0xFFFFFFFF));
            g.drawText("0" + degree, 0, slider.getTextBoxHeight()/2, textSize*2, textSize, juce::Justification::centredTop);
            g.drawText("360" + degree, width, slider.getTextBoxHeight()/2, textSize * 3, textSize, juce::Justification::centredTop);


            //Slider Thumb Pointer
            drawLinearSliderThumb(g, x, y, width,
                height, sliderPos, minSliderPos,
                maxSliderPos, style, slider);

        }
        

        void drawLinearSliderThumb(Graphics& g, int x, int y,
            int width, int height, float sliderPos,
            float minSliderPos, float maxSliderPos,
            const Slider::SliderStyle style,
            Slider& slider) override
        {

            auto centreY = (float)y + ((float)height) * 0.5f;
            auto thumbHeight = height;
            auto thumbWidth = 10.0f;
            juce::Rectangle<float> thumbPointer(sliderPos - thumbWidth/2, centreY - thumbHeight/2, thumbWidth, thumbHeight);

            g.setColour(juce::Colour(0xFFFFFFFF));
            g.fillRect(thumbPointer);
        }
    private:

    };

    //Main Component Class
    class ControlContainer : public Component{
    public:

        ControlContainer() {

            //BEAM ANGLE SLIDER

            beamAngleSlider.setSliderStyle(juce::Slider::LinearHorizontal);
            beamAngleSlider.setRange(0, 360, 5);
            beamAngleSlider.setTextValueSuffix(degree);
            beamAngleSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 80, beamAngleSlider.getTextBoxHeight());
            horizontalSliderLookAndFeel.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
            beamAngleSlider.setLookAndFeel(&horizontalSliderLookAndFeel);

            addAndMakeVisible(beamAngleSlider);


            beamAngleLabel.setText("Beam Angle", juce::dontSendNotification);
            beamAngleLabel.attachToComponent(&beamAngleSlider, true);
            addAndMakeVisible(beamAngleLabel);

        }

        void paint(Graphics& g) override {
        }

        void resized() override {

            auto area = getLocalBounds();

            auto controlGraphic = area.removeFromTop(area.getHeight());
            beamAngleSlider.setBounds(controlGraphic);
        }

        juce::Slider beamAngleSlider;


    private:

        HorizontalSliderLookAndFeel horizontalSliderLookAndFeel;

        juce::Label  beamAngleLabel;

    };


}