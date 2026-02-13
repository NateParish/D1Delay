#pragma once


struct Spark {
    float y;
    float x;
    float speed;
    bool leftSide;
};

class WidthDrawing :  public juce::Component,
                        public juce::Button::Listener,
                        private juce::TextEditor::Listener
{

private:

    D1DelayAudioProcessor& audioProcessor;
    
    std::vector<float> beatHeights = std::vector<float>(1000);
    float pi = 3.1415927;
    float curWidth = 1.0;
    float glowAlpha = 0.0f;
    float pulsePhase = 0.0f;
    std::vector<float> sparkOffsets;
    juce::Random rand;
    std::vector<Spark> sparks;

public:
    
    
    void setWidth(float width)
    {
        curWidth = width;
    }

    WidthDrawing(D1DelayAudioProcessor& p)
    : audioProcessor (p)
    {
        for (int i = 0; i < 15; ++i)
        {
            Spark s;
            s.y = rand.nextFloat();
            s.x = 0.0f;
            s.speed = 0.5f + rand.nextFloat() * 1.5f;
            s.leftSide = (i % 2 == 0);
            sparks.push_back(s);
        }
    }
    

    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();
        float compWidth = bounds.getWidth();
        float compHeight = bounds.getHeight();
        float centerX = compWidth * 0.5f;
        float barWidth = compWidth * 0.16f;
        float maxOffset = compWidth * 0.5f - barWidth;
        float offset = curWidth * maxOffset;

        float animatedAlpha = glowAlpha * 0.3f + 0.1f * std::sin(pulsePhase);
        animatedAlpha = juce::jlimit(0.0f, 1.0f, animatedAlpha);
        juce::Colour barColor = juce::Colour(16, 196, 232);

        g.fillAll(juce::Colour(10, 10, 15));

            float fadeIntensity = (1.0f - curWidth*.8);
            juce::Colour edgeGlow = barColor.withAlpha(fadeIntensity);
            juce::Colour centerFade = barColor.withAlpha(fadeIntensity * 0.8f);

            float leftInnerEdge = centerX - offset;
            float rightInnerEdge = centerX + offset;

            if ((centerX - leftInnerEdge) > 1.0f)
            {
                juce::ColourGradient glowLeft(edgeGlow, leftInnerEdge, compHeight * 0.5f,
                                              centerFade, centerX, compHeight * 0.5f, false);
                g.setGradientFill(glowLeft);
                g.fillRect(leftInnerEdge, 0.0f, centerX - leftInnerEdge, compHeight);
            }

            if ((rightInnerEdge - centerX) > 1.0f)
            {
                juce::ColourGradient glowRight(edgeGlow, rightInnerEdge, compHeight * 0.5f,
                                               centerFade, centerX, compHeight * 0.5f, false);
                g.setGradientFill(glowRight);
                g.fillRect(centerX, 0.0f, rightInnerEdge - centerX, compHeight);
            }

        const int trailLength = 6;

        for (const auto& s : sparks)
        {
            float sparkAlpha = juce::jlimit(0.0f, 1.0f, (0.2f));
            float y = compHeight * s.y;

            float barEdgeX = s.leftSide ? (centerX - offset) : (centerX + offset);
            float endX = s.leftSide
                ? juce::jmin(centerX + offset, centerX + offset/2)
                : juce::jmax(centerX - offset, centerX - offset/2);

            for (int t = 0; t < trailLength; ++t)
            {
                float age = static_cast<float>(t) / trailLength;
                float trailPos = s.x - age * 0.06f;
                if (trailPos < 0.0f || trailPos > 1.0f)
                    continue;

                float x = juce::jmap(trailPos, 0.0f, 1.0f, barEdgeX, endX);
                float alpha = sparkAlpha * (0.5f - age * 0.5f);

                float baseSize = 1.0f + (s.speed * 0.25f);
                float glowSize = baseSize + 1.0f;

                g.setColour(barColor.withAlpha(alpha * 0.7f).brighter());
                g.fillEllipse(x - glowSize * 0.5f, y - glowSize * 0.5f, glowSize, glowSize);

                g.setColour(barColor.withAlpha(alpha));
                g.fillEllipse(x - baseSize * 0.5f, y - baseSize * 0.5f, baseSize, baseSize);
            }
        }

        auto leftBar = juce::Rectangle<float>(centerX - offset - barWidth, 0.0f, barWidth, compHeight);
        auto rightBar = juce::Rectangle<float>(centerX + offset, 0.0f, barWidth, compHeight);

        g.setColour(barColor.brighter());
        g.fillRect(leftBar);
        g.fillRect(rightBar);
    }

    void resized() override {}
       
    
    bool keyPressed(const juce::KeyPress& key) override
    {
        return false;
    }
    
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override
    {
     
    }

    void buttonClicked(juce::Button* button) override
    {
      
    };

    void frameUpdate()
    {
        pulsePhase += 0.05f;
        if (pulsePhase > juce::MathConstants<float>::twoPi)
            pulsePhase -= juce::MathConstants<float>::twoPi;

        float targetAlpha = 1.0f - curWidth;
        glowAlpha += (targetAlpha - glowAlpha) * 0.05f;

        for (auto& s : sparks)
        {
            s.x += s.speed * 0.01f; // Independent of width so always moves
            if (s.x >= 1.2f)  // allow trailing past center
            {
                s.x = 0.0f;
                s.y = rand.nextFloat();
                s.speed = 0.5f + rand.nextFloat() * 1.5f;
                s.leftSide = rand.nextBool(); // optional: flip direction for variation
            }
        }

        repaint();
    }


};
