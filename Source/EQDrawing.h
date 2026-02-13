#pragma once
class EQDrawing :  public juce::Component,
                        public juce::Button::Listener,
                        private juce::TextEditor::Listener
{

private:

    D1DelayAudioProcessor& audioProcessor;
    
    std::vector<float> beatHeights = std::vector<float>(1000);
    float pi = 3.1415927;

public:

    EQDrawing(D1DelayAudioProcessor& p)
    : audioProcessor (p)
    {

    }

    void paint(juce::Graphics& g) override
    {
        juce::Rectangle<float> bounds = getLocalBounds().toFloat();
        float width = bounds.getWidth();
        float height = bounds.getHeight();

        const float minFreq = 20.0f;
        const float maxFreq = 20000.0f;
        const float minDB = -16.0f;
        const float maxDB = 12.0f;

        float sampleRate = 44100.0f; // Default sample rate
        if (auto* playHead = audioProcessor.getPlayHead())
        {
            juce::AudioPlayHead::CurrentPositionInfo posInfo;
            if (playHead->getCurrentPosition(posInfo) && posInfo.timeInSeconds > 0.0)
            {
                sampleRate = static_cast<float>(posInfo.timeInSamples / posInfo.timeInSeconds);
            }
        }

        float hpFreq = audioProcessor.getHighPass();
        float lpFreq = audioProcessor.getLowPass();

        auto freqToX = [=](float freq)
        {
            float norm = std::log10(freq / minFreq) / std::log10(maxFreq / minFreq);
            return norm * width;
        };

        auto dbToY = [=](float db)
        {
            float norm = juce::jlimit(minDB, maxDB, db);
            return juce::jmap(norm, maxDB, minDB, 0.0f, height);
        };

        // Background
        g.fillAll(juce::Colour(10, 10, 15));

        
        // dB grid
        for (float db = minDB; db <= maxDB; db += 3.0f)
        {
            float y = dbToY(db);
            g.drawHorizontalLine(static_cast<int>(y), 0.0f, width);
            g.setColour(juce::Colours::white.withAlpha(0.4f));
            g.setFont(10.0f);
            g.drawText((db > 0 ? "+" : "") + juce::String(static_cast<int>(db)) + " dB", 2, static_cast<int>(y) - 7, 40, 12, juce::Justification::left);
            g.setColour(juce::Colours::darkgrey.withAlpha(0.4f));
        }

        // Frequency response curve
        juce::Path responseCurve;
        bool isFirstPoint = true;
        for (int x = 0; x < static_cast<int>(width); ++x)
        {
            float normX = static_cast<float>(x) / width;
            float freq = minFreq * std::pow(maxFreq / minFreq, normX);

            float omega = 2.0f * juce::MathConstants<float>::pi * freq / sampleRate;
            float hpOmega = 2.0f * juce::MathConstants<float>::pi * hpFreq / sampleRate;
            float lpOmega = 2.0f * juce::MathConstants<float>::pi * lpFreq / sampleRate;

            float hpMag = omega / std::sqrt(omega * omega + hpOmega * hpOmega);
            float lpMag = 1.0f / std::sqrt(1.0f + (omega / lpOmega) * (omega / lpOmega));

            float totalMag = hpMag * lpMag;

            float gainDB = 20.0f * std::log10(juce::jlimit(0.0001f, 1.0f, totalMag));
            float y = dbToY(gainDB);

            if (isFirstPoint)
            {
                responseCurve.startNewSubPath(static_cast<float>(x), y);
                isFirstPoint = false;
            }
            else
            {
                responseCurve.lineTo(static_cast<float>(x), y);
            }
        }

        // Glow
        for (int i = 18; i >= 1; --i)
        {
            g.setColour(juce::Colour::fromRGB(160, 100, 255).withAlpha(0.001f * static_cast<float>(i)));
            g.strokePath(responseCurve, juce::PathStrokeType(3.0f + static_cast<float>(i) * 2.0f));
        }

        // Main curve
        g.setColour(juce::Colour::fromRGB(200, 130, 255));
        g.strokePath(responseCurve, juce::PathStrokeType(2.0f));

        // Marker circles for HP and LP frequencies
        auto drawFreqMarker = [&](float freq, juce::Colour colour)
        {
            float omega = 2.0f * juce::MathConstants<float>::pi * freq / sampleRate;
            float hpOmega = 2.0f * juce::MathConstants<float>::pi * hpFreq / sampleRate;
            float lpOmega = 2.0f * juce::MathConstants<float>::pi * lpFreq / sampleRate;

            float hpMag = omega / std::sqrt(omega * omega + hpOmega * hpOmega);
            float lpMag = 1.0f / std::sqrt(1.0f + (omega / lpOmega) * (omega / lpOmega));
            float totalMag = hpMag * lpMag;

            float gainDB = 20.0f * std::log10(juce::jlimit(0.0001f, 1.0f, totalMag));
            float x = freqToX(freq);
            float y = dbToY(gainDB);

            float radius = 4.0f;
            g.setColour(colour.brighter(0.8f));
            g.fillEllipse(x - radius, y - radius, radius * 2.0f, radius * 2.0f);
            g.setColour(colour.brighter(0.8f));
            g.drawEllipse(x - radius, y - radius, radius * 2.0f, radius * 2.0f, 1.0f);
        };

        drawFreqMarker(hpFreq, juce::Colour::fromRGB(200, 130, 255));
        drawFreqMarker(lpFreq, juce::Colour::fromRGB(200, 130, 255));
        g.setColour(juce::Colour(10, 10, 15));
        g.fillRect(0.f,height*.89,width,height*.11);
        g.setColour(juce::Colours::darkgrey.withAlpha(0.4f));
        juce::Array<float> freqs = { 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000 };
        for (float f : freqs)
        {
            float x = freqToX(f);
            g.drawVerticalLine(static_cast<int>(x), 0.0f, height);
            g.setColour(juce::Colours::white.withAlpha(0.4f));
            g.setFont(10.0f);
            g.drawText(juce::String(static_cast<int>(f)) + " Hz", static_cast<int>(x) + 2, static_cast<int>(height) - 15, 50, 12, juce::Justification::left);
            g.setColour(juce::Colours::darkgrey.withAlpha(0.4f));
        }

        
        
    }

    void resized() override {}
       
    
    void drawConnectedBellChain(juce::Graphics& g,
                                float graphStartX, float graphEndX, float baseY,
                                float initialHeight, float delaySpacingInBeats,
                                float samplesPerBeat, float sampleRate,
                                float feedback, float level, juce::Colour curveColour,
                                float beatCountEstimate = 32.0f, int resolution = 120, float channelLevel = 1.0f, float offset = 0, bool shortFirstPeak = false)
    {
        juce::Path path;
        float totalWidth = graphEndX - graphStartX;
        if(beatCountEstimate > 1000)
        {
            beatCountEstimate = 1000;
        }
        float beatWidth = totalWidth / beatCountEstimate;
        float sigma = beatWidth / 6.0f;

        float x = graphStartX + offset;
        float currentHeight = initialHeight;
        if(shortFirstPeak)
        {
            currentHeight = initialHeight * level;
            float root = std::pow(level,.15);
            currentHeight = initialHeight * beatHeights[0] - initialHeight * (1 - root);
            
        }

        path.startNewSubPath(graphStartX, baseY);
        int numPeaks = totalWidth / beatWidth + 1;
        float storedValue = 1;
        
        if(numPeaks > 1000)
        {
            numPeaks = 1000;
        }
        
        for(int i = 0; i < numPeaks; i++)
        {
            float curValue = storedValue * feedback * .99;
           
            if(numPeaks < beatHeights.size())
            {
                beatHeights[i] = curValue * channelLevel;
                storedValue = curValue;
            }
        };
        
        int j = 0;
        while (x < graphEndX)
        {
            float centerX = x;
            
            for (int i = 0; i <= resolution; ++i)
            {
                float localX = juce::jmap((float)i, 0.0f, (float)resolution,
                                          -beatWidth / 2.0f, beatWidth / 2.0f);
                float absoluteX = centerX + localX;

                if (absoluteX < graphStartX)
                    continue;
                if (absoluteX > graphEndX)
                    break;

                float y = std::exp(-(localX * localX) / (2.0f * sigma * sigma));
                float plotY = baseY - y * currentHeight;
    

                if (absoluteX >= path.getCurrentPosition().getX())
                    path.lineTo(absoluteX, plotY);
            }

            x += beatWidth;

            if(j < 1000)
            {
                if(beatHeights.size() > 0)
                {
                    if(j < beatHeights.size())
                    {
                        float root = std::pow(level,.15);
                        currentHeight = initialHeight * beatHeights[j] - initialHeight * (1 - root);
                        
                        currentHeight = initialHeight * beatHeights[j] - initialHeight * (1 - root);

                    }
                }
                
                if(currentHeight < 0)
                {
                    currentHeight = 0;
                }
            }
            
            j++;
        }

        // Close the path back to baseline
        path.lineTo(graphEndX, baseY);
        path.lineTo(graphStartX, baseY);
        path.closeSubPath();

        // Glow gradient fill
        juce::ColourGradient gradient(curveColour.withAlpha(1.0f),
                                      graphStartX, baseY - initialHeight,
                                      curveColour.withAlpha(0.3f),
                                      graphStartX, baseY, false);
        g.setGradientFill(gradient);
        g.fillPath(path);

        // Outline
        g.setColour(curveColour.brighter(0.2f));
        g.strokePath(path, juce::PathStrokeType(1.5f));
    }
    
    void drawBellCurve(juce::Graphics& g, float centerX, float baseY, float height, float width, int resolution = 100)
    {
        juce::Path bellPath;
        juce::Colour curveColour = juce::Colour(112, 230, 226);
        float sigma = width;  // Controls narrowness of the bell
        float startX = centerX - width / 2;
        float endX = centerX + width / 2;

        bellPath.startNewSubPath(startX, baseY); // Start from the base

        for (int i = 0; i <= resolution; ++i)
        {
            float x = juce::jmap((float)i, 0.0f, (float)resolution, startX, endX);
            float relativeX = x - centerX;
            float y = std::exp(-(relativeX * relativeX) / (2.0f * sigma * sigma));
            float plotY = baseY - y * height;

            bellPath.lineTo(x, plotY);
        }

        bellPath.lineTo(endX, baseY);  // Right base corner
        bellPath.closeSubPath();       // Close the curve

        // Glow gradient
        juce::ColourGradient gradient(curveColour.withAlpha(0.6f), centerX, baseY - height,
                                      curveColour.withAlpha(0.0f), centerX, baseY, false);
        g.setGradientFill(gradient);
        g.fillPath(bellPath);

        // Draw curve outline
        g.setColour(curveColour.brighter(0.2f));
        g.strokePath(bellPath, juce::PathStrokeType(1.5f));
    }
    
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

    }


};
