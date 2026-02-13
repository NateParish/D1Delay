#pragma once

struct DelayPreset
{
    public:
    
    DelayPreset(std::string name, float level, float highPass, float lowPass, float feedback, float time, float timeBpm, float leftLevel, float rightLevel, float width, bool leftMuted, bool rightMuted, bool pingPong, bool inBpmMode, int bpmKnobSelection)
    {
        this->name = name;
        this->level = level;
        this->feedback = feedback;
        this->time = time;
        this->timeBpm = timeBpm;
        this->bpmKnobSelection = bpmKnobSelection;
        this->inBpmMode = inBpmMode;
        this->width = width;
        this->highPass = highPass;
        this->lowPass = lowPass;
        this->leftLevel = leftLevel;
        this->rightLevel = rightLevel;
        this->leftMuted = leftMuted;
        this->rightMuted = rightMuted;
        this->pingPong = pingPong;

    }
    
    DelayPreset()
    {
    
    }
    
    std::string getName(){return name;};
    float getLevel(){return level;};
    float getHighPass(){return highPass;};
    float getLowPass(){return lowPass;};
    float getFeedback(){return feedback;};
    float getTime(){return time;};
    float getTimeBpm(){return timeBpm;};
    float getLeftLevel(){return leftLevel;};
    float getRightLevel(){return rightLevel;};
    float getWidth(){return width;};
    bool getLeftMuted(){return leftMuted;};
    bool getRightMuted(){return rightMuted;};
    bool getPingPong(){return pingPong;};
    bool getBpmMode(){return inBpmMode;};
    int getBpmKnobSelection(){return bpmKnobSelection;};
    
    void setName(std::string name){this->name = name;};
    void setLevel(float level){this->level = level;};
    void setHighPass(float highPass){this->highPass = highPass;};
    void setLowPass(float lowPass){this->lowPass = lowPass;};
    void setFeedback(float feedback){this->feedback = feedback;};
    void setTime(float time){this->time = time;};
    void setTimeBpm(float timeBpm){this->timeBpm = timeBpm;};
    void setLeftLevel(float leftLevel){this->leftLevel = leftLevel;};
    void setRightLevel(float rightLevel){this->rightLevel = rightLevel;};
    void setWidth(float width){this->width = width;};
    void setLeftMuted(bool leftMuted){this->leftMuted = leftMuted;};
    void setRightMuted(bool rightMuted){this->rightMuted = rightMuted;};
    void setPingPong(bool pingPong){this->pingPong =  pingPong;};
    void setBpmMode(bool bpmMode){this->inBpmMode = bpmMode;};
    void setBpmKnobSelection(float bpmKnobSelection){this->bpmKnobSelection = bpmKnobSelection;};
    
    
    private:
    
    std::string name = "";
    float level = 0.5;
    float highPass = 20;
    float lowPass = 20000;
    float feedback = 0.5;
    float time = 500;
    float timeBpm = 500;
    float leftLevel = 1.0;
    float rightLevel = 1.0;
    float width = 1.0;
    bool leftMuted = false;
    bool rightMuted = false;
    bool pingPong = true;
    bool inBpmMode = true;
    int bpmKnobSelection = 3;
    
};
