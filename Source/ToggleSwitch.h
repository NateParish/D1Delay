#pragma once
#include <JuceHeader.h>

class ToggleSwitch : public juce::Component
{
private:
    
    float width = 115.f;
    float heightUp = 127.f;
    float heightDown = 136;
    float curHeight = 127.f;
    float ratioUp = 115.f/127.f;
    float rationDown =115.f/136;
    float curRatio = width/curHeight;
    float scale = 1;
    
    juce::Image imageOn = juce::ImageCache::getFromMemory(BinaryData::SwitchUp_png, BinaryData::SwitchUp_pngSize);
    juce::Image imageOff = juce::ImageCache::getFromMemory(BinaryData::SwitchDown_png, BinaryData::SwitchDown_pngSize);
    
    juce::Image currentImage = imageOn;
    bool switchIsUp = true;
    
public:
    
    float getImageWidth(){return width;};
    float getImageHeight(){return curHeight;};
    void setImageWidth(int imageWidth){width = imageWidth;};
    void setImageHeight(int imageHeight){curHeight = imageHeight;};
    
 
    void paint(juce::Graphics& g) override
    {
        heightUp = getHeight();
        heightDown = getHeight();
        width = getWidth();
        
        
        if (switchIsUp)
        {
            g.drawImageWithin(imageOn, 0, 0, width * scale, heightUp * scale, juce::RectanglePlacement::centred);
        }
        else
        {
            g.drawImageWithin(imageOff, 0, getHeight()*.08, width * scale, heightDown * scale, juce::RectanglePlacement::centred);
        }
        
    }
    
    void resized() override
    {

    }
    
    void turnOn()
    {
        curHeight = heightUp;
        switchIsUp = true;
        currentImage = imageOn;
        
    }
    void turnOff()
    {
        curHeight = heightDown;
        switchIsUp = false;
        currentImage = imageOff;
    }
    
};
