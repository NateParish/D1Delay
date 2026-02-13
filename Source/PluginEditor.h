#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LoginScreen/LoginScreen.h"
#include "AppGUI.h"

//==============================================================================
/**
*/
class D1DelayAudioProcessorEditor  :    public juce::AudioProcessorEditor,
                                        public juce::Slider::Listener,
                                        public juce::Button::Listener,
                                        private juce::Timer
{
public:
    D1DelayAudioProcessorEditor (D1DelayAudioProcessor&);
    ~D1DelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void drawGlowEffect(juce::Graphics& g, const juce::Rectangle<float>& area, juce::Colour baseColor, float intensity = 1.0f);
    
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;
    void setAuthLock(AuthenticationLock* authLock){this->authLock = authLock;};
    AuthenticationLock* getAuthLock(){return authLock;};
    void loadLoginScreen();
    void loginCompleteCallback();
    void loadAppGui();
    void logout();
    
    
private:
    
    D1DelayAudioProcessor& audioProcessor;
    AuthenticationLock* authLock = audioProcessor.getAuthLock();
    std::shared_ptr<juce::Drawable> logo;
    std::shared_ptr<juce::Drawable> passVisOn;
    std::shared_ptr<juce::Drawable> passVisOff;
    
    LoginScreen loginScreen;
    AppGUI appGui;
    
    float loginWindowWidth = 400;
    float loginWindowHeight = 300;
    float loginWindowRatio = loginWindowWidth / loginWindowHeight;
    
    float stompboxSizeRatio = 385.f / 662.f;
   
    float windowScale = .9;
    float windowWidth = 792 * windowScale;
    float windowHeight = 353 * windowScale*1.1; //662
    double ratio = windowWidth/windowHeight;

    
    float effectImageWidth = 792;
    float effectImageHeight = 353;
    float effectImageRatio = 353/792;
    
    int framerate = 60;
    
    float screenWidth = 0;
    float screenHeight = 0;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (D1DelayAudioProcessorEditor)
};

