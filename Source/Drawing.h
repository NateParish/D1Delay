#pragma once
#include "CustomLF.h"
#include "math.h"

class Drawing :  public juce::Component,
                        public juce::Button::Listener,
                        private juce::TextEditor::Listener
{

private:

    D1DelayAudioProcessor& audioProcessor;
    
    std::vector<float> beatHeights = std::vector<float>(1000);
    float pi = 3.1415927;
    bool displayDelay = true;
    bool displayEq = false;
    juce::Colour litButtonColor = juce::Colour::fromRGB(255,255,255);
    juce::Colour dimButtonColor = juce::Colour::fromRGB(150,150,150);
    float levelOrigSize = 0.04 * getHeight();
    float levelCurSize = 0.04 * getHeight();
    float levelGrowSize = 0.06 * getHeight();
    float feedbackCurSize = 0.04 * getHeight();
    
    float timeSigNum;
    float timeSigDenom;
    float leftLevelOrig;
    float rightLevelOrig;
    bool leftMuted;
    bool rightMuted;
    bool bpmMode;
    bool isOn;
    int bpmSelection;
    float bpm;
    float level;
    float time;
    float feedback;
    float leftLevel;
    float rightLevel;
    float widthFromProcessor;
    float highPass;
    float lowPass;
    int highPassInt;
    int lowPassInt;
    bool pingPong = false;
    bool updatedTimeSigFlag;
    float oldTimeSigNum;
    float oldTimeSigDenom;
    
    float growTime = 0.25;
    

public:

    Drawing(D1DelayAudioProcessor& p)
    : audioProcessor (p)
    {
        updateParameters();

    }
    
    void setDisplayDelay(bool displayDelay)
    {
        this->displayDelay = displayDelay;
    }
    void setDisplayEq(bool displayEq)
    {
        this->displayEq = displayEq;
    }
    
    void makeLevelTextBig()
    {
        levelCurSize = levelGrowSize;
    }
    
    void setUpdatedTimeSigFlag(bool newFlagValue)
    {
        updatedTimeSigFlag = newFlagValue;
    }
    
    bool getUpdatedTimeSigFlag()
    {
        return updatedTimeSigFlag;
    }
    
    
    void drawRoundedBoxWithTopGap(juce::Graphics& g,
                                   float posX, float posY,
                                   float width, float height,
                                   float centerGap,
                                   float cornerRadius,
                                   juce::Colour strokeColour = juce::Colour::fromRGB(80,80,80),
                                   float strokeWidth = 1.0f)
    {
        juce::Path path;

        float left = posX;
        float right = posX + width;
        float top = posY;
        float bottom = posY + height;

        float center = posX + width / 2.0f;
        float gapHalf = centerGap / 2.0f;

        float gapLeft = center - gapHalf;
        float gapRight = center + gapHalf;

        // Start at top-left arc
        path.startNewSubPath(left + cornerRadius, top);

        // Top-left corner
        path.quadraticTo(left, top, left, top + cornerRadius);

        // Left side
        path.lineTo(left, bottom - cornerRadius);

        // Bottom-left corner
        path.quadraticTo(left, bottom, left + cornerRadius, bottom);

        // Bottom side
        path.lineTo(right - cornerRadius, bottom);

        // Bottom-right corner
        path.quadraticTo(right, bottom, right, bottom - cornerRadius);

        // Right side
        path.lineTo(right, top + cornerRadius);

        // Top-right corner
        path.quadraticTo(right, top, right - cornerRadius, top);

        // Top-right to gap right
        path.lineTo(gapRight, top);

        // Gap (skip center)
        path.startNewSubPath(gapLeft, top);

        // Gap left to top-left corner (close the top)
        path.lineTo(left + cornerRadius, top);

        // Stroke it
        g.setColour(strokeColour);
        g.strokePath(path, juce::PathStrokeType(strokeWidth));
    }
    
    void updateParameters()
    {
        leftLevelOrig = audioProcessor.getLeftLevel();
        rightLevelOrig = audioProcessor.getRightLevel();
        leftMuted = audioProcessor.getLeftMuted();
        rightMuted = audioProcessor.getRightMuted();
        level = audioProcessor.getLevel();
        time = audioProcessor.getTime();
        feedback = audioProcessor.getFeedback();
        bpmMode = audioProcessor.getBpmMode();
        bpm = audioProcessor.getBPM();
        isOn = audioProcessor.isOn();
        bpmSelection = audioProcessor.getBPMSelection();
        leftLevel = leftLevelOrig * 100;
        rightLevel = rightLevelOrig * 100;
        widthFromProcessor = audioProcessor.getWidth() * 100;
        highPass = audioProcessor.getHighPass();
        lowPass = audioProcessor.getLowPass();
        highPassInt = static_cast<int>(highPass);
        lowPassInt = static_cast<int>(lowPass);
        pingPong = audioProcessor.getPingPong();
        
        oldTimeSigNum = timeSigNum;
        oldTimeSigDenom = timeSigDenom;
        
        timeSigNum = audioProcessor.getTimeSigNumerator();
        timeSigDenom = audioProcessor.getTimeSigDenomenator();
        
        if(timeSigNum != oldTimeSigNum)
        {
            updatedTimeSigFlag = true;
        }
        if(timeSigDenom != oldTimeSigDenom)
        {
            updatedTimeSigFlag = true;
        }
        
    }

    
    void paint(juce::Graphics& g) override
    {
        float width = getWidth();
        float height = getHeight();
        
        updateParameters();

        auto formatBPM = [](float bpm) {
            return juce::String((int)std::round(bpm)) + " bpm";
        };

        juce::String bpmSettingDisplay;
        switch (bpmSelection)
        {
            case 0: bpmSettingDisplay = "1/32"; break;
            case 1: bpmSettingDisplay = "1/16"; break;
            case 2: bpmSettingDisplay = "1/8";  break;
            case 3: bpmSettingDisplay = "1/4";  break;
            case 4: bpmSettingDisplay = "1/3";  break;
            case 5: bpmSettingDisplay = "1/2";  break;
            case 6: bpmSettingDisplay = "1/1";  break;
        }

        g.setFont(height * 0.04f);
        g.setColour(juce::Colour::fromRGB(120,120,120));
        g.drawText("Mix", width * 0.124f, height * 0.1825f, width, height * 0.1f, juce::Justification::topLeft);
        g.drawText("Delay", width * 0.119f, height * 0.4125f, width, height * 0.1f, juce::Justification::topLeft);
        g.drawText("EQ", width * 0.1265f, height * 0.65f, width, height * 0.1f, juce::Justification::topLeft);
        g.drawText("Blend", width * 0.84f, height * 0.555f, width, height * 0.1f, juce::Justification::topLeft);
        g.setColour(juce::Colour::fromRGB(80,80,80));

        drawRoundedBoxWithTopGap(g, width * 0.0675f,height * 0.21f, width * (0.208f - 0.065f),height * (0.39f - 0.20f), width * 0.045f, 6.0f);
        
        drawRoundedBoxWithTopGap(g, width * 0.0675f,height * 0.44f, width * (0.208f - 0.065f),height * (0.39f - 0.20f), width * 0.05f, 6.0f);
        
        drawRoundedBoxWithTopGap(g, width * 0.0675f,height * 0.675f, width * (0.208f - 0.065f),height * (0.39f - 0.20f), width * 0.045f, 6.0f);
        
        drawRoundedBoxWithTopGap(g, width * 0.79f, height * 0.58f, width * (0.207f - 0.065f),height * (0.39f - 0.20f), width * 0.045f, 6.0f);
        
        
        g.setColour(juce::Colour::fromRGB(120,120,120));
        g.setFont(height * 0.029);

        
        g.drawText("Feedback", width * 0.147f, height * .36f, width, height * 0.1f, juce::Justification::topLeft);
        
        g.drawText("Level", width * .088, height*.36, width, height * 0.1f, juce::Justification::topLeft);
        g.drawText("Timing", width * 0.085f, height * 0.59f, width, height * 0.1f, juce::Justification::topLeft);
        
        g.drawText("Width", width * 0.155f, height * 0.59f, width, height * 0.1f, juce::Justification::topLeft);
        g.drawText("Highpass", width*.08, height*.82f, width, height * 0.1f, juce::Justification::topLeft);
        g.drawText("Lowpass", width * 0.15f, height * .82f, width, height * 0.1f, juce::Justification::topLeft);
        
        g.drawText("Left", width * 0.815f, height * 0.73f, width, height * 0.1f, juce::Justification::topLeft);
        g.drawText("Right", width * 0.882f, height * 0.73f, width, height * 0.1f, juce::Justification::topLeft);
        
        
        g.setFont(height * 0.03f);
        g.setColour(juce::Colour::fromRGB(120,120,120));
        g.drawText("display", width * 0.27f, height * 0.82f, width, height * 0.1f, juce::Justification::topLeft);
        g.setColour(juce::Colour::fromRGB(90,90,90));
        g.drawLine(width * 0.315f, height * 0.835f, width*.324f, height * 0.835f);
        g.drawLine(width * 0.324f, height * 0.806f, width*.324f, height * 0.87f);
        g.drawLine(width * 0.324f, height * 0.806f, width*.455f, height * 0.806f);
        g.drawLine(width * 0.455, height * 0.806f, width*.455f, height * 0.87f);
        g.drawLine(width * 0.324f, height * 0.87f, width*.455f, height * 0.87f);
        
        g.setFont(height * 0.03f);

        if(displayDelay && isOn)
        {
            g.setColour(litButtonColor);
        }
        else
        {
            g.setColour(dimButtonColor);
        }
        
        g.drawText("delay", width * 0.341f, height * 0.822f, width, height * 0.1f, juce::Justification::topLeft);
        if(displayEq && isOn)
        {
            g.setColour(litButtonColor);
        }
        else
        {
            g.setColour(dimButtonColor);
        }
        
        g.drawText("eq", width * 0.415f, height * 0.822f, width, height * 0.1f, juce::Justification::topLeft);
        
        g.setFont(height * 0.03f);
        if(bpmMode && isOn)
        {
            g.setColour(litButtonColor);
            g.drawText("bpm", width * 0.815f, height * 0.34f, width, height * 0.1f, juce::Justification::topLeft);
            g.setColour(dimButtonColor);
            g.drawText("ms", width * 0.885f, height * 0.34f, width, height * 0.1f, juce::Justification::topLeft);
        }
        if(!bpmMode)
        {
            g.setColour(litButtonColor);
            g.drawText("ms", width * 0.885f, height * 0.34f, width, height * 0.1f, juce::Justification::topLeft);
            g.setColour(dimButtonColor);
            g.drawText("bpm", width * 0.815f, height * 0.34f, width, height * 0.1f, juce::Justification::topLeft);

        }
        if (!isOn)
        {
            g.setColour(dimButtonColor);
            g.drawText("ms", width * 0.885f, height * 0.34f, width, height * 0.1f, juce::Justification::topLeft);
            g.setColour(dimButtonColor);
            g.drawText("bpm", width * 0.815f, height * 0.34f, width, height * 0.1f, juce::Justification::topLeft);

        }
        g.setColour(dimButtonColor);
        g.drawText("clear", width * 0.815f, height * 0.4125f, width, height * 0.1f, juce::Justification::topLeft);
        g.setFont(height * 0.022f);
        
        if(pingPong && isOn)
        {
            g.setColour(litButtonColor);
        }
        else
        {
            g.setColour(dimButtonColor);
        }
        
        g.drawText("ping pong", width * 0.873f, height * 0.415f, width, height * 0.1f, juce::Justification::topLeft);
        
        if(leftMuted && isOn)
        {
            g.setColour(litButtonColor);
        }
        else
        {
            g.setColour(dimButtonColor);
        }
        
        g.drawText("mute L", width * 0.814f, height * 0.488f, width, height * 0.1f, juce::Justification::topLeft);
        

        if(rightMuted && isOn)
        {
            g.setColour(litButtonColor);
        }
        else
        {
            g.setColour(dimButtonColor);
        }

        
        g.drawText("mute R", width * 0.88f, height * 0.488, width, height * 0.1f, juce::Justification::topLeft);
        
        
        g.setColour(dimButtonColor);
        g.setFont(height * 0.05f);
        g.drawText("Power", width * 0.835f, height * 0.215f, width, height * 0.1f, juce::Justification::topLeft);
        
        
        if(isOn)
        {
            juce::String leftLevelText = "L:";
            juce::String rightLevelText = "R:";
            juce::String widthText = "Width:";
            juce::String highPassText = "HP:";
            juce::String lowPassText =  "LP:";
            
            g.setColour(juce::Colour(220, 220, 220));
            g.setFont(height * 0.03f);
            g.drawText(leftLevelText, width * 0.61f, height * 0.33f, width, height, juce::Justification::topLeft);
            g.drawText(rightLevelText, width * 0.695f, height * 0.33f, width, height, juce::Justification::topLeft);
            
            g.setColour(juce::Colour(220, 220, 220));
            g.drawText(widthText, width * 0.465f, height * 0.76f, width, height, juce::Justification::topLeft);
            g.setFont(height*.033);
            if(pingPong && isOn)
            {
                g.setColour(juce::Colour(220, 220, 220));
                g.drawText("ping pong", width * 0.465f, height * 0.28f, width, height, juce::Justification::topLeft);
            }
            
            g.setColour(juce::Colour(220, 220, 220));
            
            g.setFont(height * 0.04f);
            if (!bpmMode)
            {
                g.drawText("Mode: ms", width * 0.64f,  height * 0.23f, width, height, juce::Justification::topLeft);
            }
            else
            {
                g.setColour(juce::Colour(220, 220, 220));
                g.drawText("Mode: bpm", width * 0.64f, height * 0.23f, width, height, juce::Justification::topLeft);
                g.setColour(juce::Colour(220, 220, 220));
                g.drawText(bpmSettingDisplay, width * 0.67f, height * 0.279f, width, height, juce::Justification::topLeft);
            }
            
            g.setFont(height * 0.04f);
            g.setColour(juce::Colour(220, 220, 220));
            juce::String timeSigNumString = juce::String(timeSigNum);
            juce::String timeSigDenomString = juce::String(timeSigDenom);
            
            g.drawText(timeSigNumString + "/" + timeSigDenomString + "    " + formatBPM(bpm), width * 0.44f, height * 0.23f, width, height, juce::Justification::topLeft);
                
            
        }
        
        
    }
    
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
    

    
    
    void resized() override
    {
        levelOrigSize = 0.04 * getHeight();
        levelCurSize = 0.04 * getHeight();
        feedbackCurSize = 0.04 * getHeight();
        levelGrowSize = 0.06 * getHeight();
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
        
    };


};
