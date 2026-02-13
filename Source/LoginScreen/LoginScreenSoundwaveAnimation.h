#pragma once

class LoginScreenSoundwaveAnimation : public juce::Component
{
    
private:
    float timerCount = 0;
    float timerCountMax = 600;

public:
    
    void paint(juce::Graphics& g) override
    {
        float height = getHeight();
        float width = getWidth();
        
        float waveAmplitude = height / 2.5f;  // Max wave height
        float waveFrequency = 1.03f;          // Base frequency
        float speed = 0.15f;                  // Animation speed

        juce::Path wavePath;
        wavePath.startNewSubPath(0, height / 2.0f); // Start at middle left

        juce::Random random;
        
        //noise-based amplitude variation
        float lastY = height / 2.0f;

        for (float x = 0; x < width; x += 6) // Move in chunks for variation
        {
            // Random variation: Smooth out randomness by interpolating with sine wave
            float noiseFactor = (random.nextFloat() * 2.0f - 1.0f) * 0.7f; // Noise between -0.7 and 0.7
            float y = height / 2.0f + (waveAmplitude * std::sin((x * waveFrequency) + (timerCount * speed)) * noiseFactor);
            
            // Smooth transitions between points
            y = (y + lastY) / 2.0f;
            wavePath.lineTo(x, y);
            lastY = y;
        }

        // Apply gradient stroke
        juce::ColourGradient gradient(
            juce::Colour(221, 230, 237).withAlpha(.5f), 0, height / 3, // Start color at top
            juce::Colour(221, 230, 237).withAlpha(0.5f), width, height * 2 / 3, // End color at bottom
            false);

        g.setGradientFill(gradient);
        g.strokePath(wavePath, juce::PathStrokeType(3.0f)); // Draw waveform

    
    g.setColour(juce::Colour(157, 178, 191).withAlpha(0.25f));
    g.strokePath(wavePath, juce::PathStrokeType(7.0f)); // Draw thicker stroke for glow
        
        
    }
    
    void resized() override
    {
        
    }
    
    void frameUpdate()
    {
        timerCount++;
        if(timerCount > timerCountMax)
        {
            timerCount = 0;
        }
    }
    
};
