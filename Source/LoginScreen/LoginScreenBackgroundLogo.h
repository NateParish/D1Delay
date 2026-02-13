#pragma once

class LoginScreenBackgroundLogo : public juce::Component
{
    
private:
    
    std::shared_ptr<juce::Drawable> logo;

public:
    
    
    void setLogoImage(std::shared_ptr<juce::Drawable> logoImage)
    {
        logo = logoImage;
    }
    
    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::yellow);
        g.fillRect(getLocalBounds());
        
        float logoSize = getHeight()*.95;
        
        g.fillAll(juce::Colour(33, 32, 30));
        if (logo != nullptr)
            {
                juce::Rectangle<float> drawArea(getWidth() / 2.0f - logoSize/2, getHeight() / 2.0f - logoSize/2,logoSize,logoSize);
                logo->drawWithin(g, drawArea, juce::RectanglePlacement::centred, 1.0f);
            }
        
    }
    
    void resized() override
    {
        
    }
    
};
