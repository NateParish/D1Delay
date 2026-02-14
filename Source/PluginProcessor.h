#pragma once

#include <JuceHeader.h>
#include "LoginScreen/Authentication.h"

//==============================================================================
/**
*/
class D1DelayAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    D1DelayAudioProcessor();
    ~D1DelayAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    float getLevel(){return level.load();};
    float getHighPass(){return highPass.load();};
    float getLowPass(){return lowPass.load();};
    float getLeftLevel(){return leftLevel.load();};
    float getRightLevel(){return rightLevel.load();};
    void setLeftLevel(float level){leftLevel.store(level);};
    void setRightLevel(float level){rightLevel.store(level);};
    float getFeedback(){return feedback.load();};
    float getTime(){return time.load();};
    void setLevel(float level) { this->level.store(level);};
    void setHighPass(float newHighPass) {
        
        highPass.store(newHighPass);
        highpassFilterLeft.setCutoffFrequency(newHighPass);
        highpassFilterRight.setCutoffFrequency(newHighPass);};

    void setLowPass(float newLowPass) {
        
        lowPass.store(newLowPass);
        lowpassFilterLeft.setCutoffFrequency(newLowPass);
        lowpassFilterRight.setCutoffFrequency(newLowPass);
    };
    
    void setFeedback(float newFeedback) { feedback.store(newFeedback);};
    void setTime(float newTime) { time.store(newTime);};
    float getWidth(){return width.load();};
    void setWidth(float width) { this->width.store(width);};
    
    bool getTimeBpm(){return timeBpm.load();};
    int getBPMSelection(){return bpmKnobSelection;};
    void setTimeBpm(int bpmSelection){
        
        if(bpmSelection == 0)
        {
            bpmKnobSelection = 0;
            this->timeBpm.store(0.125);
            
        }
        if(bpmSelection == 1)
        {
            bpmKnobSelection = 1;
            this->timeBpm.store(0.25);
            
        }
        if(bpmSelection == 2)
        {
            bpmKnobSelection = 2;
            this->timeBpm.store(0.5);
            
        }
        if(bpmSelection == 3)
        {
            bpmKnobSelection = 3;
            this->timeBpm.store(1.f);
        }
        if(bpmSelection == 4)
        {
            bpmKnobSelection = 4;
            float oneThird = 4.0f/3.0f;
            this->timeBpm.store(oneThird);
        }
        if(bpmSelection == 5)
        {
            bpmKnobSelection = 5;
            this->timeBpm.store(2.f);
        }
        if(bpmSelection == 6)
        {
            bpmKnobSelection = 6;
            this->timeBpm.store(4.f);
        }
    };
    
    float getBPM(){return bpm.load();};
    
    bool isLocked(){return authLock.isLocked();};
    AuthenticationLock* getAuthLock(){return &authLock;};
    bool isOn(){return on.load();};
    void turnOn(){on.store(true); delayBufferLeft.clear(), delayBufferRight.clear();};
    void turnOff(){on.store(false); delayBufferLeft.clear(); delayBufferRight.clear();};
    bool getBpmMode(){return inBpmMode.load();};
    void setBpmMode(bool bpmMode){this->inBpmMode.store(bpmMode);};
    bool getLeftMuted(){return leftMuted.load();};
    void setLeftMuted(bool muted){this->leftMuted.store(muted); };
    bool getRightMuted(){return rightMuted.load();};
    void setRightMuted(bool muted){this->rightMuted.store(muted); };
    bool getPingPong(){return pingPong.load();};
    void setPingPong(bool pingPong){this->pingPong.store(pingPong); };
    
    void clearDelay(){delayBufferLeft.clear(); delayBufferRight.clear();};
    juce::String getProductName(){return productName;};
    juce::String getProductVersion(){return version;};
    
    void toggleLeftMuted()
    {
        if(leftMuted.load())
        {
            leftMuted.store(false);
        }
        else
        {
            leftMuted.store(true);
        }
    }
    void toggleRightMuted()
    {
        if(rightMuted.load())
        {
            rightMuted.store(false);
        }
        else
        {
            rightMuted.store(true);
        }
    }
    void togglePingPong()
    {
        if(pingPong.load())
        {
            pingPong.store(false);
        }
        else
        {
            pingPong.store(true);
        }
    }
    
    float getTimeSigNumerator(){return timeSigNumerator.load();};
    float getTimeSigDenomenator(){return timeSigDenomenator.load();};
    float getRepaintFlag(){return repaintFlag.load();};
    void setRepaintFlag(bool repaint){repaintFlag.store(repaint);};

private:

    juce::String productName = "D1 Delay";
    juce::String version = "1_0_0";
    juce::String sku = "10001";
    
    AuthenticationLock authLock;
    juce::CriticalSection stateLock;
    
    std::atomic<bool> on = true;
    std::atomic<float> level = 0.5f;
    std::atomic<float> lowPass = 20000.f;
    std::atomic<float> highPass = 20.f;
    std::atomic<float> leftLevel = 1.0f;
    std::atomic<float> rightLevel = 1.0f;
    std::atomic<float> width = 1.0f;
    std::atomic<float> feedback = 0.5f;
    std::atomic<float> time = 0.5f;
    std::atomic<float> timeBpm = .25; // quarter note
    
    juce::AudioBuffer<float> delayBufferLeft;
    juce::AudioBuffer<float> delayBufferRight;
    int writePosition = 0;
    double currentSampleRate = 44100.0;
    std::atomic<float> bpm = 120.0;
    std::atomic<float> timeSigNumerator = 3;
    std::atomic<float> timeSigDenomenator = 4;
    std::atomic<bool> inBpmMode = true;
    std::atomic<int> bpmKnobSelection = 3;
    
    std::atomic<bool>leftMuted = false;
    std::atomic<bool>rightMuted = false;
    std::atomic<bool>pingPong = false;
    
    std::atomic<bool> repaintFlag = false;
    juce::dsp::FirstOrderTPTFilter<float> highpassFilterLeft;
    juce::dsp::FirstOrderTPTFilter<float> highpassFilterRight;
    juce::dsp::FirstOrderTPTFilter<float> lowpassFilterLeft;
    juce::dsp::FirstOrderTPTFilter<float> lowpassFilterRight;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (D1DelayAudioProcessor)
};
