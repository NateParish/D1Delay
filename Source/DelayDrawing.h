#pragma once


class DelayDrawing :  public juce::Component,
                        public juce::Button::Listener,
                        private juce::TextEditor::Listener
{

private:

    D1DelayAudioProcessor& audioProcessor;
    
    std::vector<float> beatHeightsLeft = std::vector<float>(1000);
    std::vector<float> beatHeightsRight = std::vector<float>(1000);
    float pi = 3.1415927;
    juce::Colour leftColor = juce::Colour::fromRGB(10, 230, 70);
    juce::Colour rightColor = juce::Colour::fromRGB(112, 230, 236);

  
    float level;
    float feedback;
    bool leftMuted;
    bool rightMuted;
    float leftLevel;
    float rightLevel;
    bool pingPong;
    int timeSigNum;
    int timeSigDenom;
    bool bpmMode;
    float bpmSelection;
    float timeDelayBpm;
    float timeDelay;
    float bpm;
    float sampleRate;
    
    

public:

    DelayDrawing(D1DelayAudioProcessor& p)
    : audioProcessor (p)
    {
        updateData();
        calculateHeights();
    }
    
    void setLeftColor(juce::Colour color)
    {
        leftColor = color;
    }
    void setRightColor(juce::Colour color)
    {
        rightColor = color;
    }
    
    void updateData()
    {
        level = audioProcessor.getLevel();
        feedback = audioProcessor.getFeedback();
        leftMuted = audioProcessor.getLeftMuted();
        rightMuted = audioProcessor.getRightMuted();
        leftLevel = audioProcessor.getLeftLevel();
        rightLevel = audioProcessor.getRightLevel();
        pingPong = audioProcessor.getPingPong();
        timeSigNum = audioProcessor.getTimeSigNumerator();
        if(timeSigNum < 1)
        {
            timeSigNum = 1;
        }
        if(timeSigNum > 64)
        {
            timeSigNum = 64;
        }
        timeSigDenom = audioProcessor.getTimeSigDenomenator();
        if(timeSigDenom < 1)
        {
            timeSigDenom = 4;
        }
        if(timeSigDenom > 32)
        {
            timeSigDenom = 4;
        }
        bpmMode = audioProcessor.getBpmMode();
        bpmSelection =  audioProcessor.getBPMSelection();
        timeDelayBpm = audioProcessor.getTimeBpm();
        timeDelay = audioProcessor.getTime();
        bpm = audioProcessor.getBPM();
        sampleRate = audioProcessor.getSampleRate();
        calculateHeights();
    }
    
    void calculateHeights()
    {
        
        float initialHeight = 1.0f;
        
        beatHeightsLeft[0] = initialHeight;
        beatHeightsRight[0] = initialHeight;
        float curHeightLeft = initialHeight;
        float curHeightRight = initialHeight;
        
        for(int i = 1; i < 1000; i++)
        {
            float newHeightLeft = curHeightLeft * feedback * leftLevel;
            float newHeightRight = curHeightRight * feedback * rightLevel;
            
            
            if(!leftMuted)
            {
                newHeightLeft = curHeightLeft * feedback * leftLevel;
            }
            else
            {
                newHeightLeft = 0;
            }
            if(!rightMuted)
            {
                newHeightRight = curHeightRight * feedback * rightLevel;
            }
            else
            {
                newHeightRight = 0;
            }
            beatHeightsLeft[i] = newHeightLeft;
            beatHeightsRight[i] = newHeightRight;
            curHeightLeft = newHeightLeft;
            curHeightRight = newHeightRight;
        }
        
    }
    
    void drawDelayBackground(juce::Graphics& g)
    {
            g.setColour(juce::Colour(254, 254, 254));
            g.drawLine(0, getHeight(), getWidth(), getHeight());
    }

    
    void drawOverlayedCurves(juce::Graphics& g)
    {   
    
        float width = getWidth();
        float height = getHeight();
        float graphStartX = 0;
        float graphStartY = 0;
        float graphEndX = width;
        float graphEndY = height;
        float graphWidth = graphEndX - graphStartX;
        float graphHeight = graphEndY - graphStartY;
        if(bpm < 1)
        {
            bpm = 1;
        }
        if(bpm > 999)
        {
            bpm = 999;
        }
        float beatsPerSecond = bpm/60;
        float secondsPerBeat = 1/ beatsPerSecond;
        float msPerBeat = secondsPerBeat * 1000;
        float delayTime = 500;
        float graphWidthMs = timeSigNum * msPerBeat;

        float sigma = 120 / 6.0f;
        
        g.fillAll(juce::Colour(10, 10, 15));
        
        
        g.setColour(juce::Colour(254, 254, 254));
        g.drawLine(graphStartX, graphEndY, graphEndX, graphEndY);

        g.setColour(juce::Colour(100, 100, 100));
        int measuresToDisplay = 1;
        int beatsToDisplay = timeSigNum * measuresToDisplay;
        
        float timeSigDenomAdjust = 4.0f/timeSigDenom;
        float pingPongAdj = 1.0f;
        if(pingPong)
        {
            pingPongAdj = 0.5f;
        }
        float numPeaks = 4.f;
        if(bpmMode)
        {
            if(bpmSelection == 0)
            {
                numPeaks = timeSigNum * 8 * timeSigDenomAdjust;
                delayTime = msPerBeat/8.f / timeSigDenomAdjust;
            }
            if(bpmSelection == 1)
            {
                numPeaks = timeSigNum * 4 * timeSigDenomAdjust;
                delayTime = msPerBeat/4.f / timeSigDenomAdjust;
            }
            if(bpmSelection == 2)
            {
                numPeaks = timeSigNum * 2 * timeSigDenomAdjust;
                delayTime = msPerBeat/2.f / timeSigDenomAdjust;
            }
            
            if(bpmSelection == 3)
            {
                numPeaks = timeSigNum * timeSigDenomAdjust;
                delayTime = msPerBeat / timeSigDenomAdjust;
            }
            if(bpmSelection == 4)
            {
                numPeaks = timeSigNum * 2.f/3.f * timeSigDenomAdjust;
                delayTime = msPerBeat * 3/2 / timeSigDenomAdjust;
            }
            if(bpmSelection == 5)
            {
                numPeaks = timeSigNum / 2.f * timeSigDenomAdjust;
                delayTime = msPerBeat * 2.f / timeSigDenomAdjust;
            }
            if(bpmSelection == 6)
            {
                numPeaks = timeSigNum / 4.f * timeSigDenomAdjust;
                delayTime = msPerBeat * 4.f / timeSigDenomAdjust;
            }
        
            
        }
        else
        {
            delayTime = timeDelay * 1000;
            numPeaks = graphWidthMs / (delayTime);
        }
        
        if(pingPong)
        {
            numPeaks = numPeaks/2;
            delayTime = delayTime * 2;
        }
       
        if(numPeaks < 1)
        {
            numPeaks = 1;
        }
        if(numPeaks > 100)
        {
            numPeaks = 100;
        }
        g.setColour(juce::Colour::fromRGB(40,40,40));
        g.drawLine(0, 0, graphEndX, 0, 2.0f);
        
        g.setColour(juce::Colour::fromRGB(40,40,40));
        g.drawLine(0, height*.5, graphEndX, height*.5, 1.0f);
        
        g.setColour(juce::Colour::fromRGB(28,28,28));
        g.drawLine(0, height*.25, graphEndX, height*.25, 1.0f);
        g.drawLine(0, height*.75, graphEndX, height*.75, 1.0f);
        
        g.setColour(juce::Colour::fromRGB(100,100,100));
        
        
        float beatWidth = (graphEndX - graphStartX) / (numPeaks);
        if(pingPong)
        {
            beatWidth = beatWidth/2;
        }
        float gridBeatWidth = (graphEndX - graphStartX) / timeSigNum;
        float drawX = graphStartX;
        for (int i = 0; i <= beatsToDisplay; ++i)
        {
            g.drawLine(drawX, graphStartY, drawX, graphEndY, 2.0f);
            drawX += gridBeatWidth;
        }
        
        
        g.setColour(juce::Colour(254, 254, 254));
        float measureWidth = (graphEndX - graphStartX) / measuresToDisplay;
        drawX = graphStartX;
        for (int i = 0; i <= measuresToDisplay; ++i)
        {
            g.drawLine(drawX, graphStartY, drawX, graphEndY, 2.0f);
            drawX += measureWidth;
        }
        
        float timeMS = juce::jlimit(1.0f, 2000.0f, timeDelay * 1000.0f);

        if(timeMS < 1)
        {
            timeMS = 1;
        }

        float delaySpacingInBeats = 1.0f;
        float beatCountEstimate = 32.0f;

        
        
        if (bpmMode)
        {
            static const float bpmOptions[] = { 1.0f/32, 1.0f/16, 1.0f/8, 1.0f/4, 1.0f/3, 1.0f/2, 1.0f };
            int bpmSelectionInt = juce::jlimit(0, 6, static_cast<int>(bpmSelection));
            delaySpacingInBeats = bpmOptions[bpmSelectionInt] * 4;
            beatCountEstimate = beatsToDisplay / delaySpacingInBeats;
        }
        else
        {
            float msPerBeat = 60000.0f / bpm;
            delaySpacingInBeats = timeMS / msPerBeat;
            beatCountEstimate = beatsToDisplay / delaySpacingInBeats;
            if (beatCountEstimate > 1000)
            {
                beatCountEstimate = 1000;
            }
        }

        float leftOffset = 0;
        bool leftShortFirstPeak = false;

        if(pingPong)
        {
            leftOffset =  - delaySpacingInBeats * beatWidth / 2;
            leftShortFirstPeak = true;
        }
        
        float centerX = 150;
        
        float k = beatsToDisplay * beatsToDisplay/2.5;
        sigma = beatWidth / k;


        if(numPeaks < 1)
        {
            numPeaks = 1;
        }
        
        float numPeaksLeft = numPeaks;
        if(pingPong)
        {
            numPeaksLeft++;
        }
        
        for(int i = 0; i < numPeaksLeft; i++)
        {
            
            if(pingPong && i > 0)
            {
                centerX = i * graphWidth/numPeaks - (graphWidth/numPeaks/2);
            }
            else
            {
                centerX = i * graphWidth/numPeaks;
            }
            
            if(i > 0)
            {
                drawSingleCurve(g, graphWidth, graphHeight, graphStartX, graphStartY,
                                centerX, beatHeightsLeft[i] * level, sigma, leftColor, 400);
            }
            else
            {
                drawSingleCurve(g, graphWidth, graphHeight, graphStartX, graphStartY,
                                centerX, beatHeightsLeft[i], sigma, leftColor, 400);
            }
        }
        
        for(int i = 0; i < numPeaks; i++)
        {
            centerX = i * graphWidth/numPeaks;
            if(i > 0)
            {
                drawSingleCurve(g, graphWidth, graphHeight, graphStartX, graphStartY,
                                centerX, beatHeightsRight[i] * level, sigma, rightColor, 400);
            }
            else
            {
                drawSingleCurve(g, graphWidth, graphHeight, graphStartX, graphStartY,
                                centerX, beatHeightsRight[i], sigma, rightColor, 400);
            }
        }
        
        
        
        

    }
    
    void drawSingleCurve(juce::Graphics& g, float displayWidth, float displayHeight,
                         float displayX, float displayY, float centerX, float drawHeight,
                         float sigma, juce::Colour color, int resolution)
    {

        
        float startX = centerX - displayWidth / 2;
        float maxDrawHeight = displayHeight * drawHeight;
        float baseY = displayY + displayHeight;


        juce::Path bellPath;
        bellPath.startNewSubPath(startX, baseY);

        for (int i = 0; i <= resolution; ++i)
        {
            float x = startX + (i / static_cast<float>(resolution)) * displayWidth;
            float relativeX = x - centerX;
            float y = std::exp(-(relativeX * relativeX) / (2.0f * sigma * sigma));
            float plotY = baseY - y * maxDrawHeight;

            bellPath.lineTo(x, plotY);
        }

        bellPath.lineTo(startX + displayWidth, baseY);
        bellPath.closeSubPath();

        juce::ColourGradient gradient(color.withAlpha(0.9f), centerX, baseY - maxDrawHeight,
                                      color.withAlpha(0.2f), centerX, baseY, false);
        g.setGradientFill(gradient);
        g.fillPath(bellPath);

        g.setColour(color.brighter(0.2f));
        g.strokePath(bellPath, juce::PathStrokeType(1.5f));
    }

    void paint(juce::Graphics& g) override
    {
        drawDelayBackground(g);
        drawOverlayedCurves(g);

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
            currentHeight = initialHeight * beatHeightsLeft[0] - initialHeight * (1 - root);
            
        }


        path.startNewSubPath(graphStartX, baseY);
        float numPeaks = totalWidth / beatWidth + 1;
        float storedValue = 1;
        
        if(numPeaks > 1000)
        {
            numPeaks = 1000;
        }
        
        for(int i = 0; i < numPeaks; i++)
        {
            float curValue = storedValue * feedback * .99;
           
            if(numPeaks < beatHeightsLeft.size())
            {
                beatHeightsLeft[i] = curValue * channelLevel;
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
                if(beatHeightsLeft.size() > 0)
                {
                    if(j < beatHeightsLeft.size())
                    {
                        float root = std::pow(level,.15);
                        currentHeight = initialHeight * beatHeightsLeft[j] - initialHeight * (1 - root);
                        
                        currentHeight = initialHeight * beatHeightsLeft[j] - initialHeight * (1 - root);

                    }
                }
                
                if(currentHeight < 0)
                {
                    currentHeight = 0;
                }
            }
            
            j++;
        }

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
