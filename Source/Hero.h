/*
  ==============================================================================

    Footer.h
    Created: 26 Apr 2022 2:49:17pm
    Author:  jiang

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>            

#if JUCE_ANDROID
#include "AndroidMotionManager.h"
#endif

#include "Helper.h"


namespace Gui {
    std::string degree = "º";

    class MapBaseComponent : public juce::Component
    {
    public:
        MapBaseComponent() {}

        void paint(juce::Graphics& g) override
        {
            auto width = getWidth();
            auto height = getHeight();
            auto dialBounds = 20.0f;
            auto radius = (float)juce::jmin(width / 2, height / 2) - dialBounds;
            auto centreX = (float)getX() + (float)width * 0.5f;
            auto centreY = (float)getY() + (float)height * 0.5f;

            //Centering Dial
            auto rx = centreX - radius;
            auto ry = centreY - radius;
            auto rw = radius * 2.0f;

            //Fill Dial Base
            g.setColour(juce::Colour(0xFF4F6367));
            g.fillEllipse(rx, ry, rw, rw);
            //Add Degree Labels

            g.resetToDefaultState();
            auto textSize = 13.0f;
            g.setFont(juce::Font("Montserrat", textSize, juce::Font::bold));
            g.setColour(juce::Colour(0xFFFFFFFF));
            g.drawText("N 0"+ degree+"/360"+ degree, 0, -(radius + textSize / 2), width, height, juce::Justification::centred);

            g.addTransform(juce::AffineTransform::rotation(juce::MathConstants<float>::pi, centreX, centreY));
            g.drawText("S", 0, -(radius + textSize / 2), width, height, juce::Justification::centred);

            g.addTransform(juce::AffineTransform::rotation(-juce::MathConstants<float>::pi / 2, centreX, centreY));
            g.drawText("E", 0, -(radius + textSize / 2), width, height, juce::Justification::centred);

            g.addTransform(juce::AffineTransform::rotation(-juce::MathConstants<float>::pi, centreX, centreY));
            g.drawText("W", 0, -(radius + textSize / 2), width, height, juce::Justification::centred);


            auto radiusSrc = (float)juce::jlimit(3.0f, 8.0f, (float)juce::jmin(getWidth() / 12, getHeight() / 12) - dialBounds);
            auto rxSrc = centreX - radiusSrc;
            auto rySrc = centreY - radiusSrc;
            auto rwSrc = radiusSrc * 2.0f;


            g.resetToDefaultState();
            for (int i = 0; i < noSources; i++) {
                g.setColour(juce::Colour(0xFFFE5F55));
                g.fillEllipse(rxSrc + sources[i].x*100, rySrc + sources[i].y*100, rwSrc, rwSrc);
                //g.drawText(sources[i].sourceName, 0, textSize, rxSrc + sources[i].x * 100, height, juce::Justification::centred);
            }

        }

        struct source {
            int x = 0;
            int y = 0;
            juce::String sourceName = "";
        };
        //Define two known sources
        int noSources = 2;
        source sources[2];
    private:
        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MapBaseComponent)
    };

    //Beam Dial Appearance Class
    class BeamDialLookAndFeel : public juce::LookAndFeel_V4{
    public:
        void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override {
            ///Draw Dial Base

            //Setting Dial Size
            auto dialBounds = 20.0f;
            auto radius = (float)juce::jmin(width / 2, height / 2) - dialBounds;
            auto centreX = (float)x + (float)width * 0.5f;
            auto centreY = (float)y + (float)height * 0.5f;
            //Centering Dial
            auto rx = centreX - radius;
            auto ry = centreY - radius;
            auto rw = radius * 2.0f;

            //Fill Dial Base
            //g.setColour(juce::Colour(0xFF4F6367));
            //g.fillEllipse(rx, ry, rw, rw);

            //Angle at which the dial points
            auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

            //Beam Dial pointer
            juce::Path headArrow,headArrowCenter, beamformer;

            beamformer.addPieSegment(rx, ry, rw, rw, -0.125 * juce::MathConstants<float>::pi,0.125*juce::MathConstants<float>::pi, 0);
            beamformer.applyTransform(juce::AffineTransform::rotation(angle, centreX, centreY));

            g.setColour(juce::Colour(0x809A9A9A));
            g.fillPath(beamformer);


        }
    private:

    };


    class HeadDialLookAndFeel : public juce::LookAndFeel_V4 {
    public:
        void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override {
            ///Draw Dial Base

            //Setting Dial Size
            auto radius = (float)juce::jmin(width / 2, height / 2);
            auto centreX = (float)x + (float)width * 0.5f;
            auto centreY = (float)y + (float)height * 0.5f;
            //Centering Dial
            auto rx = centreX - radius;
            auto ry = centreY - radius;
            auto rw = radius * 2.0f;



            //Angle at which the dial points
            auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

            //Beam Dial pointer
            juce::Path headArrow, headArrowCenter;


            auto circleSize = rw / 4.5;
            auto circleCentreX = centreX - circleSize / 2;
            auto circleCentreY = centreY - circleSize / 2;
            headArrowCenter.addEllipse(circleCentreX, circleCentreY, circleSize, circleSize);

            juce::Line<float> line(juce::Point<float>(0, radius*0.8), juce::Point<float>(0, -radius));
            headArrow.addArrow(line, radius / 8, radius * 0.75, radius / 3);
            headArrow.applyTransform(juce::AffineTransform::translation(centreX, centreY).rotated(angle, centreX, centreY));


            g.setColour(juce::Colour(0xFFFFFFFF));
            g.strokePath(headArrowCenter, PathStrokeType(radius / 4));
            g.fillPath(headArrow);
            g.setColour(juce::Colour(0xFFFE5F55));
            g.fillPath(headArrowCenter);
        }
    private:

    };


    //Main Component Class
    class HeaderContainer : public Component, public juce::Timer {
    public:

        HeaderContainer() {
            //TOP HALF

            addAndMakeVisible(mapBase);
            //BEAM DIAL

            beamDial.setSliderStyle(juce::Slider::Rotary);
            beamDial.setRange(0, 360, 5);
            beamDial.setRotaryParameters(0, 2*juce::MathConstants<float>::pi, false);
            beamDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 160, headDial.getTextBoxHeight());

            //Setting the look and feel of the beam dial
            beamDial.setLookAndFeel(&beamDialLookAndFeel);

            addAndMakeVisible(beamDial);


            //HEAD ORIENTATION Dial
            headDial.setSliderStyle(juce::Slider::Rotary);
            headDial.setRange(0, 360);
            headDial.setRotaryParameters(0, 2 * juce::MathConstants<float>::pi, false);
            headDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 160, headDial.getTextBoxHeight());

            //Setting the look and feel of the head dial
            headDial.setLookAndFeel(&headDialLookAndFeel);

            addAndMakeVisible(headDial);


            //If it's an android device start the android sensor read
            #if JUCE_ANDROID
                        compass.start();
                        startTimer(1);
            #endif
            
        }

        void paintOverChildren(Graphics& g) override {     


        }

        void resized() override {

            auto area = getLocalBounds();

            auto headerFooterHeight = area.getHeight();

            auto heroGraphic = area.removeFromTop(headerFooterHeight);

            mapBase.setBounds(heroGraphic.reduced(0));
            beamDial.setBounds(heroGraphic.reduced(0));

            juce::Rectangle<int> dialContainer((heroGraphic.getWidth() - heroGraphic.getHeight()) / 2, 0, heroGraphic.getHeight(), heroGraphic.getHeight());
            headDial.setBounds(dialContainer.reduced(heroGraphic.getHeight()/2.5));

        }

        //This function repeatedly updates the compass object
        void timerCallback() override {
            #if JUCE_ANDROID
            compass.Update();
            compass.lowPassFilter();
            float heading = compass.calculateHeading();
            AffineTransform t = AffineTransform::rotation(heading,beamDial.getWidth()/2, beamDial.getHeight()/2);
            mapBase.setTransform(t);
            beamDial.setTransform(t);
            headDial.setTransform(t);
            #endif


        }


        juce::Slider beamDial;
        juce::Slider headDial;

        MapBaseComponent mapBase;

        //Source Locations
        
    private:
        //Define class of the beam dial appearance
        BeamDialLookAndFeel beamDialLookAndFeel;
        HeadDialLookAndFeel headDialLookAndFeel;


        #if JUCE_ANDROID
        MotionManager compass;
        HelperFunctions dialHelper;
        #endif

    };


}