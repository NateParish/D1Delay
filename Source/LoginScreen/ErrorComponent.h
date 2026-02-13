#pragma once
#include "Authentication.h"
#include "../CustomLF.h"

class ErrorMessageComponent : public juce::Component,
                    public juce::Button::Listener,
                    private juce::TextEditor::Listener
{
    
private:
    juce::String websiteURL = "";
    juce::URL websiteLink = juce::URL("");
    AuthenticationLock* authLock;
    std::function<void()> okButtonCallback;
    
    ButtonStyle1LF okButtonLF;
    LoginScreenBackgroundLogo logo;
    LoginScreenSoundwaveAnimation soundWave;
    TransparancyLayer blackLayer;
    juce::Label headerLabel;
    juce::Label message;
    juce::TextButton okButton;
    juce::HyperlinkButton websiteLinkButton;
    
    
    std::thread networkThread;
    int timerCount = 0;
    int timerCountMax = 600;
    
    
    
public:
    
    ErrorMessageComponent()
    {
        
        
        headerLabel.setText("Error Logging In", juce::dontSendNotification);
        headerLabel.setBounds(0, 15, getWidth(), 30);
        headerLabel.setFont(juce::FontOptions(28.0f));
        headerLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        headerLabel.setJustificationType(juce::Justification::centred);
        
        message.setText("Please Try Again", juce::dontSendNotification);
        message.setBounds(0, 15, getWidth(), 30);
        message.setFont(juce::FontOptions(16.0f));
        message.setColour(juce::Label::textColourId, juce::Colours::white);
        message.setJustificationType(juce::Justification::centred);
        
        
        okButton.setButtonText("Back To Login");
        okButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        okButton.setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
        okButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        okButton.addListener(this);
        okButton.setLookAndFeel(&okButtonLF);
        
        websiteLinkButton.setButtonText(websiteURL);

        websiteLinkButton.setURL(websiteLink);
        websiteLinkButton.setFont(juce::FontOptions("Arial", 16.0f, juce::Font::bold), false, juce::Justification::centred);
        websiteLinkButton.setColour(juce::HyperlinkButton::textColourId, juce::Colour(157, 178, 191));
        
        addAndMakeVisible(&logo);
        addAndMakeVisible(&blackLayer);
        addAndMakeVisible(&headerLabel);
        addAndMakeVisible(&message);
        addAndMakeVisible(&websiteLinkButton);
        addAndMakeVisible(&okButton);
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
        headerLabel.setBounds(0,getHeight()*-.1,getWidth(),getHeight());
        headerLabel.setFont(juce::FontOptions(getHeight()*.1));
        message.setBounds(0,0,getWidth(),getHeight());
        message.setFont(juce::FontOptions(getHeight()*.07));
        websiteLinkButton.setBounds(0, getHeight()*-.1, getWidth(),getHeight()*.06);
        okButton.setBounds(getWidth()*.3,getHeight()*.7,getWidth()*.4,getHeight()*.1);
        
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
            okButtonCallback();
            return true;
        }
        return false;
    }

    void assignKeyboard()
    {
        grabKeyboardFocus();
        okButton.setWantsKeyboardFocus(true);
        okButton.grabKeyboardFocus();
    }
    
    
    void setAuthLock(AuthenticationLock* authLock)
    {
        this->authLock = authLock;
    }
    
    void frameUpdate()
    {
        
    }
    
};
