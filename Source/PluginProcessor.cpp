#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
D1DelayAudioProcessor::D1DelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    lowpassFilterLeft.setType(juce::dsp::FirstOrderTPTFilterType::lowpass);
    lowpassFilterRight.setType(juce::dsp::FirstOrderTPTFilterType::lowpass);
    highpassFilterLeft.setType(juce::dsp::FirstOrderTPTFilterType::highpass);
    highpassFilterRight.setType(juce::dsp::FirstOrderTPTFilterType::highpass);
}

D1DelayAudioProcessor::~D1DelayAudioProcessor()
{
}

//==============================================================================
const juce::String D1DelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool D1DelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool D1DelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool D1DelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double D1DelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int D1DelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int D1DelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void D1DelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String D1DelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void D1DelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void D1DelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    delayBufferLeft.setSize(getTotalNumInputChannels(), (int)(sampleRate * 2.0)); // 2 sec max delay
    delayBufferLeft.clear();
    delayBufferRight.setSize(getTotalNumInputChannels(), (int)(sampleRate * 2.0)); // 2 sec max delay
    delayBufferRight.clear();
    writePosition = 0;
    
    
    juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = getTotalNumOutputChannels();
    
    
    highpassFilterLeft.prepare({ sampleRate, (juce::uint32)samplesPerBlock, (juce::uint32)getTotalNumOutputChannels() });
    highpassFilterRight.prepare({ sampleRate, (juce::uint32)samplesPerBlock, (juce::uint32)getTotalNumOutputChannels() });
    lowpassFilterLeft.prepare({ sampleRate, (juce::uint32)samplesPerBlock, (juce::uint32)getTotalNumOutputChannels() });
    lowpassFilterRight.prepare({ sampleRate, (juce::uint32)samplesPerBlock,
        (juce::uint32)getTotalNumOutputChannels() });
    
    highpassFilterLeft.setCutoffFrequency(highPass.load());
    highpassFilterRight.setCutoffFrequency(highPass.load());
    lowpassFilterLeft.setCutoffFrequency(lowPass.load());
    lowpassFilterRight.setCutoffFrequency(lowPass.load());
    
}

void D1DelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    delayBufferLeft.clear();
    delayBufferRight.clear();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool D1DelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


void D1DelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (authLock.isLocked()) return;
    if (!isOn()) return;
    
    juce::ScopedNoDenormals noDenormals;
    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels(); // use this instead of getTotalNumInputChannels()
    // Get current BPM
    if (auto* playHead = getPlayHead())
    {
        juce::AudioPlayHead::CurrentPositionInfo posInfo;
        if (playHead->getCurrentPosition(posInfo))
        {
            float prevBPM = bpm.load();
            float curBpm = posInfo.bpm;
            if(curBpm < 1.0f)
            {
                curBpm = 1.0f;
            }
            bpm.store(curBpm);
            
            if(curBpm != prevBPM)
            {
                bpm.store(curBpm);
                repaintFlag.store(true);
            }
            
            float prevNumerator = timeSigNumerator.load();
            float prevDenominator = timeSigDenomenator.load();
            
            if(prevNumerator != posInfo.timeSigNumerator)
            {
                timeSigNumerator.store(posInfo.timeSigNumerator);
                repaintFlag.store(true);
            }
            if(prevDenominator != posInfo.timeSigDenominator)
            {
                timeSigDenomenator.store(posInfo.timeSigDenominator);
                repaintFlag.store(true);
            }
            
        }
    }
    
    if(numChannels == 1)
    {
        float delayTime = time.load(); // 0.0 to 1.0
        float beatLengthSeconds = 60.0f / bpm.load();
        float curTimeBpm = timeBpm.load();
        float curFeedback = feedback.load();
        float curLevel = level.load();
        float curLeftLevel = leftLevel.load();
        
        float* leftInput = buffer.getWritePointer(0);
        float* delayDataLeft = delayBufferLeft.getWritePointer(0);
        int delayBufferSizeLeft = delayBufferLeft.getNumSamples();
        int localWritePos = writePosition;
        
        float delayedSampleLeft = 0;
        float inLeft = 0;
        float leftBlend = 0;
        int readPos = 0;
        
        float curDenom = timeSigDenomenator.load();
        float denomFactor = curDenom / 4;
        
        float effectiveTimeBpm = curTimeBpm * denomFactor;
        
        int delayInSamples = 0;
        if (inBpmMode)
            delayInSamples = static_cast<int>(beatLengthSeconds * currentSampleRate * effectiveTimeBpm);
        else
            delayInSamples = static_cast<int>(delayTime * currentSampleRate);
        
        
        delayInSamples = std::min(delayInSamples, delayBufferSizeLeft - 1);
        
        for (int i = 0; i < numSamples; ++i)
        {
            readPos = (localWritePos - delayInSamples + delayBufferSizeLeft) % delayBufferSizeLeft;
            
            delayedSampleLeft = delayDataLeft[readPos];
            inLeft = leftInput[i];
            leftBlend = inLeft + delayedSampleLeft;
            
            leftBlend = lowpassFilterLeft.processSample(0,highpassFilterLeft.processSample(0,leftBlend));
            delayDataLeft[localWritePos] = inLeft + (leftBlend * curFeedback * 0.9f);
            leftInput[i] = (inLeft + leftBlend * curLevel * curLeftLevel);
            
            localWritePos = (localWritePos + 1) % delayBufferSizeLeft;
            
        }
        
        writePosition = (writePosition + numSamples) % delayBufferLeft.getNumSamples(); // assumes same size
    }
    
    
    if(numChannels > 1)
    {
        float delayTime = time.load(); // 0.0 to 1.0
        float beatLengthSeconds = 60.0f / bpm.load();
        float curTimeBpm = timeBpm.load();
        float curFeedback = feedback.load();
        float curLevel = level.load();
        float curLeftLevel = leftLevel.load();
        float curRightLevel = rightLevel.load();
        float curWidth = width.load();
        bool curPingPong = pingPong.load();
        
        float* leftInput = buffer.getWritePointer(0);
        float* rightInput = buffer.getWritePointer(1);
        float* delayDataLeft = delayBufferLeft.getWritePointer(0);
        float* delayDataRight = delayBufferRight.getWritePointer(0);
        int delayBufferSizeLeft = delayBufferLeft.getNumSamples();
        int localWritePos = writePosition;
        
        float delayedSampleLeft = 0;
        float delayedSampleRight = 0;
        float inLeft = 0;
        float inRight = 0;
        float leftBlend = 0;
        float rightBlend = 0;
        float oppMixAmount = (1 - curWidth);
        int readPos = 0;
                
        // Adjust beat subdivision interpretation for 1/8 note pulses
        float curDenom = timeSigDenomenator.load();
        float denomFactor = curDenom / 4;
    
        float effectiveTimeBpm = curTimeBpm * denomFactor;
        
        int delayInSamples = 0;
        if (inBpmMode)
            delayInSamples = static_cast<int>(beatLengthSeconds * currentSampleRate * effectiveTimeBpm);
        else
            delayInSamples = static_cast<int>(delayTime * currentSampleRate);
        
        
        delayInSamples = std::min(delayInSamples, delayBufferSizeLeft - 1);
        
        
        for (int i = 0; i < numSamples; ++i)
        {
            readPos = (localWritePos - delayInSamples + delayBufferSizeLeft) % delayBufferSizeLeft;
            
            delayedSampleLeft = delayDataLeft[readPos];
            delayedSampleRight = delayDataRight[readPos];
            inLeft = leftInput[i];
            inRight = rightInput[i];
            leftBlend = 0;
            rightBlend = 0;
            
            if(width > 0)
            {
                
                leftBlend = delayedSampleLeft + oppMixAmount * delayedSampleRight/2;
                rightBlend = delayedSampleRight + oppMixAmount * delayedSampleLeft/2;
                
            }
            else
            {
                leftBlend = (delayedSampleLeft + delayedSampleRight)/2;
                rightBlend = (delayedSampleRight + delayedSampleLeft)/2;
            }
            
            if(curPingPong)
            {
                delayDataLeft[localWritePos] = (rightBlend * curFeedback * 0.9f);
                delayDataRight[localWritePos] = inRight + (leftBlend * curFeedback * 0.9f);
            }
            else
            {
                delayDataLeft[localWritePos] = inLeft + (leftBlend * curFeedback * 0.9f);
                delayDataRight[localWritePos] = inRight + (rightBlend * curFeedback * 0.9f);
            }
            
            leftBlend = lowpassFilterLeft.processSample(0,highpassFilterLeft.processSample(0,leftBlend));
            rightBlend = lowpassFilterRight.processSample(0, highpassFilterRight.processSample(0, rightBlend));
            
            leftInput[i] = (inLeft + leftBlend * curLevel * curLeftLevel);
            rightInput[i] = (inRight + rightBlend * curLevel * curRightLevel);
            
            localWritePos = (localWritePos + 1) % delayBufferSizeLeft;
            
            
        }
        
        writePosition = (writePosition + numSamples) % delayBufferLeft.getNumSamples(); // assumes same size
    }
}


//==============================================================================
bool D1DelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* D1DelayAudioProcessor::createEditor()
{
    return new D1DelayAudioProcessorEditor (*this);
}

//==============================================================================
void D1DelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ScopedLock lock (stateLock);
    juce::MemoryOutputStream stream(destData, true);
    stream.writeFloat(level.load());
    stream.writeFloat(highPass.load());
    stream.writeFloat(lowPass.load());
    stream.writeFloat(feedback.load());
    stream.writeFloat(time.load());
    stream.writeFloat(timeBpm.load());
    stream.writeFloat(leftLevel.load());
    stream.writeFloat(rightLevel.load());
    stream.writeFloat(width.load());
    stream.writeBool(leftMuted.load());
    stream.writeBool(rightMuted.load());
    stream.writeBool(pingPong.load());
    stream.writeBool(inBpmMode.load());
    stream.writeInt(bpmKnobSelection.load());
    stream.writeFloat(lowPass.load());
    stream.writeFloat(highPass.load());
    stream.writeBool(on.load());
}

void D1DelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ScopedLock lock (stateLock);
    juce::MemoryInputStream stream(data, static_cast<size_t>(sizeInBytes), false);
    level.store(stream.readFloat());
    highPass.store(stream.readFloat());
    lowPass.store(stream.readFloat());
    feedback.store(stream.readFloat());
    time.store(stream.readFloat());
    timeBpm.store(stream.readFloat());
    leftLevel.store(stream.readFloat());
    rightLevel.store(stream.readFloat());
    width.store(stream.readFloat());
    leftMuted.store(stream.readBool());
    rightMuted.store(stream.readBool());
    pingPong.store(stream.readBool());
    inBpmMode.store(stream.readBool());
    bpmKnobSelection.store(stream.readInt());
    lowPass.store(stream.readFloat());
    highPass.store(stream.readFloat());
    on.store(stream.readBool());
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new D1DelayAudioProcessor();
}
