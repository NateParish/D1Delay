#pragma once
#include <JuceHeader.h>

class FloorImage : public juce::Component
{
private:
    
    juce::Image floorImage = juce::ImageCache::getFromMemory(BinaryData::Floor_png, BinaryData::Floor_pngSize);
    
    
public:
    
 
    void paint(juce::Graphics& g) override
    {

        if (floorImage.isValid())
        {
            g.drawImage(floorImage, getBounds().toFloat(), juce::RectanglePlacement::stretchToFit);
        }

    }
    
    void resized() override
    {

    }
    
};
