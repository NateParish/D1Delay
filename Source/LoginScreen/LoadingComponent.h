#pragma once
#include "LoginScreenSoundwaveAnimation.h"
#include "../CustomLF.h"

class LoadingComponent : public juce::Component,
                    public juce::Button::Listener,
                    private juce::TextEditor::Listener
{
    
private:
    juce::String websiteURL = "";
    juce::URL websiteLink = juce::URL("");
    std::function<void()> cancelButtonCallback;
    ButtonStyle1LF cancelButtonLF;
    
    std::shared_ptr<juce::Drawable> logoImage;
    LoginScreenBackgroundLogo logo;
    LoginScreenSoundwaveAnimation soundWave;
    TransparancyLayer blackLayer;
    juce::Label loadingLabel;
    juce::TextButton cancelButton;
    juce::HyperlinkButton websiteLinkButton;
    
    int timerCount = 0;
    int timerCountMax = 600;
     
    
public:
    
    LoadingComponent()
    {
        
        loadingLabel.setText("Loading...", juce::dontSendNotification);
        loadingLabel.setBounds(0, 15, getWidth(), 30);
        loadingLabel.setFont(juce::FontOptions(getHeight()*.12));
        loadingLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        loadingLabel.setJustificationType(juce::Justification::centred);
        
        cancelButton.setButtonText("Cancel");
        cancelButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        cancelButton.setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
        cancelButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        cancelButton.addListener(this);
        cancelButton.setLookAndFeel(&cancelButtonLF);
        
        websiteLinkButton.setButtonText(websiteURL);

        websiteLinkButton.setURL(websiteLink);
        websiteLinkButton.setFont(juce::FontOptions("Arial", 16.0f, juce::Font::bold), false, juce::Justification::centred);
        websiteLinkButton.setColour(juce::HyperlinkButton::textColourId, juce::Colour(157, 178, 191));
        
        addAndMakeVisible(&logo);
        addAndMakeVisible(&blackLayer);
        addAndMakeVisible(&soundWave);
        addAndMakeVisible(&loadingLabel);
        addAndMakeVisible(&cancelButton);
        addAndMakeVisible(&websiteLinkButton);
        blackLayer.setRGBA(0,0,0,100);
        
    }
    
    void paint(juce::Graphics& g) override
    {
    }
    
    
    void setLogoImage(std::shared_ptr<juce::Drawable> logoImage)
    {
        this->logoImage = logoImage;
        logo.setLogoImage(logoImage);
    }
    
    void resized() override
    {
        
        
        logo.setBounds(getBounds());
        blackLayer.setBounds(getBounds());
        loadingLabel.setFont(juce::FontOptions(getHeight()*.12));
        soundWave.setBounds(getBounds());
        loadingLabel.setBounds(0,-20,getWidth(),getHeight());
        cancelButton.setBounds(getWidth()*.5 - getWidth()*.15,getHeight()*.7,getWidth()*.3,getHeight()*.1);
        websiteLinkButton.setBounds(0, getHeight()-30, getWidth(),25);
    }
    
    void buttonClicked(juce::Button* button) override
    {
        if (button == &cancelButton)
        {
            cancelButtonCallback();
        }
    };
    
    void setCancelButtonCallback(std::function<void()> newCallback)
    {
        cancelButtonCallback = std::move(newCallback);
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
    
    void assignKeyboard()
    {
        grabKeyboardFocus();
    }
    
    
};
