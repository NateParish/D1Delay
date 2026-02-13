#pragma once
#include "JuceHeader.h"

class VisOnOffImage : public juce::Component
{
    
private:
    
    std::shared_ptr<juce::Drawable> visOnImage;
    std::shared_ptr<juce::Drawable> visOffImage;
    std::shared_ptr<juce::Drawable> curImage;
    
    bool on = true;

public:
    
    VisOnOffImage()
    {

    }
    
    void setImages(std::shared_ptr<juce::Drawable> visOnImage, std::shared_ptr<juce::Drawable> visOffImage)
    {
        this->visOnImage = visOnImage;
        this->visOffImage = visOffImage;

    }
    
    void paint(juce::Graphics& g) override
    {

        juce::Rectangle<float> drawArea(0, 0, getWidth(), getHeight());
        juce::Rectangle<float> drawArea2(0, 1, getWidth(), getHeight());
        
        if(on)
        {
            
            
            if (visOnImage != nullptr)
                {
                    visOnImage->drawWithin(g, drawArea, juce::RectanglePlacement::centred, 1.0f);
                }
            else{
                DBG("NULL IMAGE POINTER - VIS ON BUTTON");
            }
        }
        if(!on)
        {
            
            if (visOffImage != nullptr)
                {
                    visOffImage->drawWithin(g, drawArea2, juce::RectanglePlacement::centred, 1.0f);
                }
            else{
                DBG("NULL IMAGE POINTER - VIS OFF BUTTON");
            }
        }

        
        
    }
    
    void resized() override
    {
        
    }
    
    void switchToVisOn()
    {
        on = true;
    }
    void switchToVisOff()
    {
        on = false;
    }
    
};
