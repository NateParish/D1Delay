#pragma once
#include "Authentication.h"


class NoLicenseComponent : public juce::Component,
                    public juce::Button::Listener,
                    private juce::TextEditor::Listener
{
    
private:
    juce::String websiteURL = "";
    juce::URL websiteLink = juce::URL("");
    AuthenticationLock* authLock;
    std::function<void()> okButtonCallback;
    
    LoginScreenBackgroundLogo logo;
    LoginScreenSoundwaveAnimation soundWave;
    TransparancyLayer blackLayer;
    juce::Label headerLabel;
    juce::Label message;
    juce::Label message2Left;
    juce::Label message2Right;
    juce::TextButton okButton;
    juce::HyperlinkButton websiteLinkButton;
    juce::HyperlinkButton websiteLinkButton2;
    
    std::thread networkThread;
    int timerCount = 0;
    int timerCountMax = 600;
    
    
    
public:
    
    NoLicenseComponent()
    {
        
        headerLabel.setText("No Product License Found", juce::dontSendNotification);
        headerLabel.setBounds(0, 15, getWidth(), 30);
        headerLabel.setFont(juce::FontOptions(28.0f));
        headerLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        headerLabel.setJustificationType(juce::Justification::centred);
        
        message.setText("No Product License Found", juce::dontSendNotification);
        message.setBounds(0, 15, getWidth(), 30);
        message.setFont(juce::FontOptions(16.0f));
        message.setColour(juce::Label::textColourId, juce::Colours::white);
        message.setJustificationType(juce::Justification::centred);
        
        message2Left.setText("Please visit ", juce::dontSendNotification);
        message2Left.setBounds(0, 15, getWidth(), 30);
        message2Left.setFont(juce::FontOptions(16.0f));
        message2Left.setColour(juce::Label::textColourId, juce::Colours::white);
        message2Left.setJustificationType(juce::Justification::centred);
        
        message2Right.setText("to obtain one", juce::dontSendNotification);
        message2Right.setBounds(0, 15, getWidth(), 30);
        message2Right.setFont(juce::FontOptions(16.0f));
        message2Right.setColour(juce::Label::textColourId, juce::Colours::white);
        message2Right.setJustificationType(juce::Justification::centred);
        
        okButton.setButtonText("Back To Login");
        okButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        okButton.setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
        okButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        okButton.addListener(this);
        
        websiteLinkButton.setButtonText(websiteURL);
        websiteLinkButton.setURL(websiteLink);
        websiteLinkButton.setFont(juce::FontOptions("Arial", 16.0f, juce::Font::bold), false, juce::Justification::centred);
        websiteLinkButton.setColour(juce::HyperlinkButton::textColourId, juce::Colour(157, 178, 191));
        
        websiteLinkButton2.setButtonText(websiteURL);
        websiteLinkButton2.setURL(websiteLink);
        websiteLinkButton2.setFont(juce::FontOptions("Arial", 16.0f, juce::Font::bold), false, juce::Justification::centred);
        websiteLinkButton2.setColour(juce::HyperlinkButton::textColourId, juce::Colour(157, 178, 191));
  
        addAndMakeVisible(&logo);
        addAndMakeVisible(&blackLayer);
        addAndMakeVisible(&headerLabel);
        addAndMakeVisible(&message2Left);
        addAndMakeVisible(&message2Right);
        addAndMakeVisible(&okButton);
        addAndMakeVisible(&websiteLinkButton);
        addAndMakeVisible(&websiteLinkButton2);
        blackLayer.setRGBA(0,0,0,100);
       
    }

    void setLogoImage(std::shared_ptr<juce::Drawable> logoImage)
    {
        this->logo.setLogoImage(logoImage);
        logo.setLogoImage(logoImage);

    }
    
    void paint(juce::Graphics& g) override
    {
    }
    
    void loadMessages()
    {
        headerLabel.setText(authLock->getErrorMessage(), juce::NotificationType::dontSendNotification);
    }
    
    void resized() override
    {
        
        logo.setBounds(getBounds());
        blackLayer.setBounds(getBounds());
        soundWave.setBounds(getBounds());
        headerLabel.setBounds(0,-75,getWidth(),getHeight());
        message.setBounds(0,0,getWidth(),getHeight());
        message2Left.setBounds(-100,-10,getWidth(), getHeight());
        message2Right.setBounds(110,-10,getWidth(),getHeight());
        okButton.setBounds(getWidth()*.5 - 75,getHeight()*.7,150,40);
        websiteLinkButton.setBounds(0, getHeight()-30, getWidth(),25);
        websiteLinkButton2.setBounds(getWidth()*.5-60,getHeight()*.5 - 19,120,20);
        
        
    }
    
    void buttonClicked(juce::Button* button) override
    {
        if (button == &okButton)
        {
            okButtonCallback();
        }
    };
    
    void setOkButtonCallback(std::function<void()> newCallback)
    {
        okButtonCallback = std::move(newCallback);
    }
    
    bool keyPressed(const juce::KeyPress& key) override
    {
        

        if (key == juce::KeyPress::returnKey) // Check if Enter key was pressed
        {
            okButton.triggerClick();

            return true;
        }
        return false;
    }

    void assignKeyboard()
    {
        grabKeyboardFocus();
        websiteLinkButton.setWantsKeyboardFocus(true);
        websiteLinkButton.grabKeyboardFocus(); // go to website if enter key is pressed
    }
    
    
    void setAuthLock(AuthenticationLock* authLock)
    {
        this->authLock = authLock;
    }
    
    void frameUpdate()
    {
        soundWave.frameUpdate();
        timerCount++;
        if(timerCount > timerCountMax)
        {
            timerCount = 0;
        }
    }
    
};
