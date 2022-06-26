

#pragma once

#include "Hero.h"
#include "Control.h"
#include "Helper.h"

//==============================================================================

//Label Appearance Class
class LabelLookAndFeel : public juce::LookAndFeel_V4
{
public:
    //LOOK AND FEEL FOR LABEL
    void drawLabel(Graphics& g, Label& label) override {
        auto labelText = label.getText();
        auto labelArea{ label.getLocalBounds().toFloat() };
        const float cornerSize{ labelArea.getHeight() /2};

        g.setColour(juce::Colour(0xFF4F6367));
        g.fillRoundedRectangle(labelArea, cornerSize);

        g.setColour(juce::Colour(0xFFFFFFFF));
        g.drawFittedText(labelText, labelArea.getX(), labelArea.getY(), labelArea.getWidth(), labelArea.getHeight(), juce::Justification::centred, 2);
    }

    void fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& editor) {
        auto labelArea{ editor.getLocalBounds().toFloat() };
        const float cornerSize{ labelArea.getHeight() / 2 };

        g.setColour(juce::Colour(0xFF4F6367));
        g.fillRoundedRectangle(labelArea, cornerSize);
    }

    void drawTextEditorOutline(Graphics& g, int width, int height, TextEditor& editor) {
        auto labelArea{ editor.getLocalBounds().toFloat().reduced(1.0f)};
        const float cornerSize{ labelArea.getHeight() / 2 };

        g.setColour(juce::Colour(0xFFFFFFFF));
        g.drawRoundedRectangle(labelArea, cornerSize, 2.0f);
    }

private:

};


//Button Appearance Class
class ButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    //LOOK AND FEEL FOR BUTTON
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
        bool, bool isButtonDown) override {

        auto buttonArea = button.getLocalBounds();
        auto edge = 2;

        buttonArea.removeFromLeft(edge);
        buttonArea.removeFromTop(edge);


        auto offset = isButtonDown ? -edge / 2 : -edge;
        buttonArea.translate(offset, offset);

        g.setColour(juce::Colour(0xFF4C5359));
        g.fillRoundedRectangle((float)buttonArea.getX(), (float)buttonArea.getY(), (float)buttonArea.getWidth(), (float)buttonArea.getHeight(), (float)buttonArea.getHeight() / 2);
    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown) override
    {
        auto font = getTextButtonFont(button, button.getHeight());
        g.setFont(font);
        g.setColour(button.findColour(button.getToggleState() ? juce::TextButton::textColourOnId
            : juce::TextButton::textColourOffId)
            .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

        auto yIndent = juce::jmin(4, button.proportionOfHeight(0.3f));
        auto cornerSize = juce::jmin(button.getHeight(), button.getWidth()) / 2;

        auto fontHeight = juce::roundToInt(font.getHeight() * 0.6f);
        auto leftIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
        auto rightIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
        auto textWidth = button.getWidth() - leftIndent - rightIndent;

        auto edge = 4;
        auto offset = isButtonDown ? edge / 2 : 0;

        if (textWidth > 0)
            g.drawFittedText(button.getButtonText(),
                leftIndent + offset, yIndent + offset, textWidth, button.getHeight() - yIndent * 2 - edge,
                juce::Justification::centred, 2);
    
    }

private:

};




class MainContentComponent   : public juce::Component, public juce::Slider::Listener, public juce::LookAndFeel_V4, public juce::OSCReceiver, private juce::OSCReceiver::Listener<juce::OSCReceiver::MessageLoopCallback>
{
public:

    //MAIN COMPONENT
    MainContentComponent()
    {
        //CALIBRATE NORTH BUTTON
        calibrateButton.setLookAndFeel(&buttonLookAndFeel);
        addAndMakeVisible(calibrateButton);
        calibrateButton.setTriggeredOnMouseDown(true);
        calibrateButton.setRepeatSpeed(1, 1, 1);
        calibrateButton.onClick = [this] { calibrateButtonClicked(); };
        //Initialise button as disabled until connected
        calibrateButton.setEnabled(false);


        //RENDER HERO COMPONENT
        addAndMakeVisible(headerContainer);
        headerContainer.beamDial.addListener(this);
        headerContainer.headDial.addListener(this);
        headerContainer.headDial.setEnabled(false);


        //BEAM ANGLE SLIDER
        addAndMakeVisible(beamAngleSlider);
        beamAngleSlider.beamAngleSlider.addListener(this);


        formLabel.setEditable(false);
        formLabel.setText("Hearing Aid Details:", juce::dontSendNotification);
        formLabel.setFont(juce::Font("Montserrat", 15.00f, juce::Font::bold));
        formLabel.setJustificationType(juce::Justification::bottomLeft);
        addAndMakeVisible(formLabel);

        //PORT CONNECT
        ipAddressField.setEditable(true, true, true);
        ipAddressField.setLookAndFeel(&labelLookAndFeel);
        addAndMakeVisible(ipAddressField);

        portNumberField.setEditable(true, true, true);
        portNumberField.setLookAndFeel(&labelLookAndFeel);
        addAndMakeVisible(portNumberField);

        connectButton.setLookAndFeel(&buttonLookAndFeel);
        addAndMakeVisible(connectButton);
        connectButton.onClick = [this] { connectButtonClicked(); };

        updateConnectionStatusLabel();
        addAndMakeVisible(connectionStatusLabel);

        //FIX BEAM RADIO BUTTON
        fixBeamToHead.onClick = [this] { toggleFixedBeam(&fixBeamToHead);   };
        addAndMakeVisible(fixBeamToHead);
        fixBeamToHead.setEnabled(false);


        pageLabel.setEditable(false);
        pageLabel.setText(pageTitle, juce::dontSendNotification);
        pageLabel.setFont(juce::Font("Montserrat", 30.00f, juce::Font::bold));
        pageLabel.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(pageLabel);



        //tell the component to listen for OSC messages matching this address:
        receiver.addListener(this);

        setSize (375, 667);
    }



    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colour(0xFF7A9E9F));
    }

    void resized() override
    {
        auto area = getLocalBounds();


        auto headerFooterHeight = area.getHeight() / 2;
        calibrateButton.setBounds(area.removeFromTop(30.0f).reduced(area.getWidth()/2.5, 5));
        headerContainer.setBounds(area.removeFromTop(headerFooterHeight).reduced(20));



        auto footerArea = area.removeFromBottom(headerFooterHeight);

        auto footerItemHeight = 50;
        auto footerItemMargin = 40;

        auto labelItemHeight = 25.0f;
        auto ipAddressLength = area.getWidth() / 2;
        auto portLength = area.getWidth() / 4;

        beamAngleSlider.setBounds(footerArea.removeFromTop(footerItemHeight).reduced(footerItemMargin, 0));

        fixBeamToHead.setBounds(footerArea.removeFromTop(labelItemHeight).reduced(footerItemMargin, 0));

        formLabel.setBounds(footerArea.removeFromTop(labelItemHeight*1.5).reduced(footerItemMargin, 0));

        auto formContainerHeight = labelItemHeight * 3;

        auto formContainerArea = footerArea.removeFromTop(formContainerHeight).reduced(footerItemMargin, formContainerHeight /2.75);
        formContainer.setBounds(formContainerArea);


        ipAddressField.setBounds(formContainerArea.removeFromLeft(ipAddressLength));
        portNumberField.setBounds(formContainerArea.removeFromRight(portLength));

        connectButton.setBounds(footerArea.removeFromTop(labelItemHeight).reduced(footerItemMargin * 3, 0));
        connectionStatusLabel.setBounds(footerArea.removeFromTop(labelItemHeight).reduced(footerItemMargin, 0));


        pageLabel.setBounds(footerArea.removeFromBottom(120.00f).reduced(0, 30.0f));

    }


    void calibrateButtonClicked() {
        calibrateNorthValue = OSCHeadDirection;
        if (!sender.send("/steer/cal/out", 1))
            showConnectionErrorMessage("Error: Failed to send message. Check hearing aid connection.");
        if (!sender.send("/steer/cal/out", 0))
            showConnectionErrorMessage("Error: Failed to send message. Check hearing aid connection.");
    }

    void toggleFixedBeam(juce::Button* button) {
        if (button->getToggleState()){
            fixedBeamDirectionValue = (headDirectionValue - rawBeamDirection) % 360;
        }
        else {
            fixedBeamDirectionValue = 0;
        }
        rawBeamDirection = helper.convertPositiveDegree(headDirectionValue - fixedBeamDirectionValue) % 360;
        beamAngleSlider.beamAngleSlider.setValue(rawBeamDirection, juce::sendNotificationAsync);
        headerContainer.beamDial.setValue(rawBeamDirection, juce::sendNotificationAsync);
        if (!sender.send("/steer/fixbeam/out", button->getToggleState()))
            showConnectionErrorMessage("Error: Failed to send message. Check hearing aid connection.");
    }

    void sliderValueChanged(juce::Slider* slider) override
    {

            //If Beam is fixed to head then sliders will not function;
            /// SENDING OSC MESSAGE OF A CHOSEN BEAM LOCATION WILL ALWAYS BE IN FRONT OF THE HEAD
            if (fixBeamToHead.getToggleState()) {
                rawBeamDirection = helper.convertPositiveDegree(headDirectionValue - fixedBeamDirectionValue) % 360;
                beamAngleSlider.beamAngleSlider.setValue(rawBeamDirection, juce::dontSendNotification);
                headerContainer.beamDial.setValue(rawBeamDirection, juce::dontSendNotification);
            }

            //// SENDING OSC MESSAGE OF FIXED BEAM LOCATION REGARDLESS OF HEAD ORIENTATION
            else if (slider == &beamAngleSlider.beamAngleSlider || slider == &headerContainer.beamDial || slider == &headerContainer.headDial) {

                //Change Value of opposing slider that wasn't touched
                if (slider == &beamAngleSlider.beamAngleSlider) {
                    rawBeamDirection = beamAngleSlider.beamAngleSlider.getValue();
                    headerContainer.beamDial.setValue(rawBeamDirection, juce::dontSendNotification);
                }
                else if (slider == &headerContainer.beamDial) {
                    rawBeamDirection = headerContainer.beamDial.getValue();
                    beamAngleSlider.beamAngleSlider.setValue(rawBeamDirection, juce::dontSendNotification);
                }


                if (isConnected()){
                    // Send value as OSC message
                    if (!sender.send("/steer/source", (int)rawBeamDirection))
                        showConnectionErrorMessage( "Error: Failed to send message. Check hearing aid connection.");
                }
            }

    }

    void oscMessageReceived(const juce::OSCMessage& message) override
    {

        //Check the type of data received
        //HEAD TRACKER
        if (message.getAddressPattern() == "/main/out/rot") {
            //TODO CHECK NEGATIVES
            int headOrientation = message[0].getFloat32();
            OSCHeadDirection = headOrientation;


            headDirectionValue = headOrientation;
            headerContainer.headDial.setValue(headDirectionValue);
        }

        //adding locations to array
        else if (message.getAddressPattern() == "/tascarpos" && message[0].isString())
        {
            //Loop two length source array
            for (int i = 0; i < headerContainer.mapBase.noSources; i++) {
                if (headerContainer.mapBase.sources[i].sourceName != message[0].getString()) {
                    if (headerContainer.mapBase.sources[i].sourceName.isEmpty()) {
                        headerContainer.mapBase.sources[i].sourceName = message[0].getString();
                        headerContainer.mapBase.sources[i].x = message[1].getFloat32();
                        headerContainer.mapBase.sources[i].y = message[2].getFloat32();
                        break;
                    }
                }
            }

        }
    }



    juce::OSCReceiver receiver;
private:
    HelperFunctions helper;
    LabelLookAndFeel labelLookAndFeel;
    ButtonLookAndFeel buttonLookAndFeel;


    Gui::HeaderContainer headerContainer;
    Gui::ControlContainer beamAngleSlider;

    juce::TextButton formContainer;

    juce::Label formLabel;

    juce::Label pageLabel;
    juce::String pageTitle = "Control Center";

    //Radio Button For fixing beam to head
    juce::ToggleButton fixBeamToHead{ "Fix Beam To Head" };


    juce::TextButton calibrateButton{ "Set North" };

    int calibrateNorthValue = 0;
    //These values correspond to the raw angles obtained by the OSC message or Slider
    int rawBeamDirection = 0;
    int OSCHeadDirection = 0;
    //These values correspond to the altered angles after calibration
    int fixedBeamDirectionValue = 0;
    int headDirectionValue = 0;







    //IP AND PORT CONNECTION
    //==============================================================================
    juce::Label ipAddressField{ {}, "IP Address:" };
    juce::Label portNumberField{ {}, "Port:" };
    juce::TextButton connectButton{ "CONNECT" };
    juce::Label connectionStatusLabel;



    bool isValidOscPort(int port) const
    {
        return port > 0 && port < 65536;
    }
    bool isValidOscIp(juce::String ip) {
        return helper.validateIP(ip.toStdString());
    }

    void connectButtonClicked()
    {
        if (!isConnected())            
            connect();
        else
            disconnect();

        updateConnectionStatusLabel();  
    }

    void connect()
    {
        auto portToConnect = portNumberField.getText().getIntValue();  
        juce::String ipToConnect = ipAddressField.getText();

        if (!isValidOscPort(portToConnect) || !isValidOscIp(ipToConnect))
        {
            handleInvalidPortNumberEntered();
            return;
        }

        if (sender.connect(ipToConnect, portToConnect))
        {
            currentPortNumber = portToConnect;
            currentIpAddress = ipToConnect;
            connectButton.setButtonText("Disconnect");
        }
        else                                                            
        {
            handleConnectError(portToConnect);
        }

        // specify here on which UDP port number to receive incoming OSC messages
        if (!receiver.connect(7778))
            showConnectionErrorMessage("Error: could not connect to UDP port 7778.");


        fixBeamToHead.setEnabled(true);
        calibrateButton.setEnabled(true);
    }

    void disconnect()
    {
        if (sender.disconnect() && receiver.disconnect())
        {
            currentPortNumber = -1;
            currentIpAddress = "";
            connectButton.setButtonText("Connect");

            calibrateButton.setEnabled(false);
            fixBeamToHead.setEnabled(false);
            headerContainer.headDial.setEnabled(false);
        }
        else
        {
            handleDisconnectError();   
        }
    }
    bool isConnected() const
    {
        return currentPortNumber != -1;
    }

    void updateConnectionStatusLabel()
    {
        juce::String text = "Status: ";

        if (isConnected())
            text += "Connected.";
        else
            text += "Disconnected. Connect Hearing Aid.";

        auto textColour = isConnected() ? juce::Colours::green : juce::Colours::red;

        connectionStatusLabel.setText(text, juce::dontSendNotification);
        connectionStatusLabel.setFont(juce::Font(15.00f, juce::Font::bold));
        connectionStatusLabel.setColour(juce::Label::textColourId, textColour);
        connectionStatusLabel.setJustificationType(juce::Justification::bottomLeft);


    }

    void showConnectionErrorMessage(const juce::String& messageText)
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
            "Connection error",
            messageText,
            "OK");
    }
    void handleConnectError(int failedPort)
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
            "OSC Connection error",
            "Error: could not connect to port " + juce::String(failedPort),
            "OK");
    }
    void handleDisconnectError()
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
            "Unknown error",
            "An unknown error occured while trying to disconnect from UDP port.",
            "OK");
    }
    void handleInvalidPortNumberEntered()
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
            "Invalid port number and/or IP Address",
            "Error: you have entered an invalid UDP port number and/or IP Address.",
            "OK");
    }
     


    juce::OSCSender sender;

    int currentPortNumber = -1;
    juce::String currentIpAddress = "";


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
