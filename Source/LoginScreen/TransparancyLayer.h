#pragma once

class TransparancyLayer : public juce::Component
{
    
private:
    
    float red = 0;
    float green = 0;
    float blue = 0;
    float alpha = 1;
    
   juce::Colour color = juce::Colour::fromRGBA(red,green,blue,alpha);
    

public:
    
    
    void paint(juce::Graphics& g) override
    {
        g.fillAll(color);
    }
    
    void resized() override
    {
        
    }
    
    void setRed(float red)
    {
        this->red = red;
        color = juce::Colour::fromRGBA(red,green,blue,alpha);
    }
    void setGreen(float green)
    {
        this->green = green;
        color = juce::Colour::fromRGBA(red,green,blue,alpha);
    }
    void setBlue(float blue)
    {
        this->blue = blue;
        color = juce::Colour::fromRGBA(red,green,blue,alpha);
    }
    
    void setAlpha(float alpha)
    {
        this->alpha = alpha;
        color = juce::Colour::fromRGBA(red,green,blue,alpha);
    }
    void setRGBA(float red, float green, float blue, float alpha)
    {
        this->red = red;
        this->green = green;
        this->blue = blue;
        this->alpha = alpha;
        color = juce::Colour::fromRGBA(red,green,blue,alpha);
    }
        
};
