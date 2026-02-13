#pragma once

#include "LoginScreenSoundwaveAnimation.h"

class WelcomeComponent : public juce::Component,
                    public juce::Button::Listener,
                    private juce::TextEditor::Listener
{
    
private:
    
    float animationStarted = false;
    float curFrame = 0;
    float framesTextGrowing = 25;
    float framesFadeOut = 100;
    float framesPauseAtEnd = 10;
    float totalFrames;
    
    float totalLengthInSeconds;
    float textStartSize = 20;
    float textSizeIncrease = 0;
    float textEndSize = 60;
    
    int timerCount = 0;
    int timerCountMax = 600;
    
    bool textFinishedGrowing = false;
    
    juce::String websiteURL = "";
    juce::URL websiteLink = juce::URL("");
    std::function<void()> animationCompleteCallback;
    
    std::shared_ptr<juce::Drawable> logoImage;
    LoginScreenBackgroundLogo logo;
    LoginScreenSoundwaveAnimation soundWave;
    TransparancyLayer blackLayer;
    TransparancyLayer blackFadeout;
    juce::Label welcomeLabel;
    juce::HyperlinkButton websiteLinkButton;
    
    juce::ImageButton emptyButton;
    
    
    std::thread networkThread;

    

public:
    
    WelcomeComponent()
    {
        
        welcomeLabel.setText("Welcome!", juce::dontSendNotification);
        welcomeLabel.setBounds(0, 15, getWidth(), 30);
        welcomeLabel.setFont(juce::FontOptions(28.0f));
        welcomeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        welcomeLabel.setJustificationType(juce::Justification::centred);
        
        websiteLinkButton.setButtonText(websiteURL);

        websiteLinkButton.setURL(websiteLink);
        websiteLinkButton.setFont(juce::FontOptions("Arial", 16.0f, juce::Font::bold), false, juce::Justification::centred);
        websiteLinkButton.setColour(juce::HyperlinkButton::textColourId, juce::Colour(157, 178, 191));
        websiteLinkButton.setWantsKeyboardFocus(false);
        
        emptyButton.setWantsKeyboardFocus(true);
        
        addAndMakeVisible(&logo);
        addAndMakeVisible(&blackLayer);
        addAndMakeVisible(&soundWave);
        addAndMakeVisible(&blackFadeout);
        addAndMakeVisible(&welcomeLabel);
        addAndMakeVisible(&websiteLinkButton);
        addAndMakeVisible(&emptyButton);
        
        blackLayer.setRGBA(0,0,0,100);
        blackFadeout.setRGBA(0,0,0,0);
        
    }
    
    void setLogoImage(std::shared_ptr<juce::Drawable> logoImage)
    {
        this->logoImage = logoImage;
        logo.setLogoImage(logoImage);
        
    }
    
    void paint(juce::Graphics& g) override
    {

    }
    
    void resized() override
    {
        logo.setBounds(getBounds());
        blackLayer.setBounds(getBounds());
        soundWave.setBounds(getBounds());
        blackFadeout.setBounds(getBounds());
        welcomeLabel.setBounds(0,-20,getWidth(),getHeight());
        websiteLinkButton.setBounds(0, getHeight()-30, getWidth(),25);
    }
    
    void buttonClicked(juce::Button* button) override
    {
        if(button == &emptyButton)
        {
            DBG("EMPTY BUTTON CLICK!");
        }
    };
    

    void frameUpdate()
    {

        if(animationStarted)
        {
            soundWave.frameUpdate();
            timerCount++;
            curFrame++;
            if(textStartSize + textSizeIncrease < textEndSize)
            {
                textSizeIncrease = textSizeIncrease + (textEndSize - textStartSize)/framesTextGrowing;
                welcomeLabel.setFont(juce::FontOptions(textStartSize + textSizeIncrease));
            }
            if(curFrame > framesTextGrowing && curFrame < framesTextGrowing + framesFadeOut)
            {
                float newAlpha =(curFrame-framesTextGrowing) * (255/framesFadeOut);
                if(newAlpha >= 255)
                {
                    newAlpha = 255;
                }
                
                
                blackFadeout.setAlpha(newAlpha);
            }
            
            if(timerCount > timerCountMax)
            {
                timerCount = 0;
            }
            
            if(curFrame >= framesTextGrowing + framesFadeOut + framesPauseAtEnd)
            {
                animationStarted = false;
                animationCompleteCallback();
            }
        }
        
    }
    
    void startAnimation()
    {
        welcomeLabel.setFont(juce::FontOptions(textStartSize));
        blackLayer.setAlpha(100);
        blackFadeout.setAlpha(0);
        curFrame = 0;
        textSizeIncrease = 0;
        animationStarted = true;
        
    }
    
    
    void setAnimationCompleteCallback(std::function<void()> newCallback)
    {
        animationCompleteCallback = std::move(newCallback);
    }
    
    void assignKeyboard()
    {
        grabKeyboardFocus();
    }
    
};
