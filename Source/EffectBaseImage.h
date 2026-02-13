#pragma once
#include <JuceHeader.h>

class EffectBaseImage : public juce::Component
{
private:
    
    float width = 875;
    float height = 356;
    float ratio = 356.f/875.f;
    float scale = .9;
    
    juce::Image imageOn = juce::ImageCache::getFromMemory(BinaryData::D1Delay_png, BinaryData::D1Delay_pngSize);
    juce::Image imageOff = juce::ImageCache::getFromMemory(BinaryData::D1Delay_png, BinaryData::D1Delay_pngSize);
    
    juce::Image currentImage = imageOn;
    
    
public:
    
    float getImageWidth(){return width;};
    float getImageHeight(){return height;};
    void setImageWidth(int imageWidth){width = imageWidth;};
    void setImageHeight(int imageHeight){height = imageHeight;};
    
 
    void paint(juce::Graphics& g) override
    {

        
        if (currentImage.isValid())
        {
            g.drawImageWithin(currentImage, getWidth() * (1 - scale)*.5, getHeight()*.6 * (1 - scale), getWidth()*.9, getHeight() * .9, juce::RectanglePlacement::centred);
        }
        
    }
    
    void resized() override
    {

    }
    
    void turnOn()
    {
        currentImage = imageOn;
    }
    
    void turnOff()
    {
        currentImage = imageOn;
    }
    
};
