#pragma once

#include <JuceHeader.h>
#include "EffectBaseImage.h"
#include "FloorImage.h"
#include "DrawableComponent.h"
#include "ThemeColors.h"
#include "CustomLF.h"
#include "MenuBar.h"
#include "ToggleSwitch.h"
#include "Drawing.h"
#include "EQDrawing.h"
#include "WidthDrawing.h"
#include "DelayDrawing.h"
#include "MenuBar.h"
#include "Components.h"
#include "Presets.h"


class AppGUI :  public juce::Component,
                public juce::Slider::Listener,
                public juce::Button::Listener,
                public juce::TextEditor::Listener
{
    
private:
    
    D1DelayAudioProcessor& audioProcessor;
    juce::CriticalSection presetLock;
    FloorImage floor;
    
    ThemeColors themeColors;
  
    juce::Component dummyFocusCatcher;
    float textHeight = 25;
    
    ToggleSwitch onOffSwitchImage;
    ToggleSwitch bpmSwitchImage;
    
    juce::ImageButton bpmSelectButton;
    
    KnobImage levelLF;
    KnobImage lowPassLF;
    KnobImage highPassLF;
    KnobImage feedbackLF;
    KnobImage timeLF;
    KnobImage bpmLF;
    KnobImage leftLevelKnobLF;
    KnobImage rightLevelKnobLF;
    KnobImage widthKnobLF;
    TextBoxLF textBoxLF;
    juce::TextEditor timeTextBox;
    Drawing drawLayer;
    EQDrawing eqDrawing;
    DelayDrawing delayDrawing;
    EQDrawing eqFadeDrawing;
    DelayDrawing delayFadeDrawing;
    WidthDrawing widthDrawing;
    
    juce::Slider levelKnob;
    juce::Slider lowPassKnob;
    juce::Slider highPassKnob;
    juce::Slider leftLevelKnob;
    juce::Slider rightLevelKnob;
    juce::Slider feedbackKnob;
    juce::Slider timeKnob;
    juce::Slider bpmKnob;
    juce::Slider widthKnob;
    juce::ImageButton bpmButton;
    juce::ImageButton msButton;
    juce::ImageButton muteLeftButton;
    juce::ImageButton muteRightButton;
    juce::ImageButton clearButton;
    juce::ImageButton pingPongButton;
    
    juce::ImageButton displayDelayButton;
    juce::ImageButton displayEQButton;
    juce::ImageButton displayWidthButton;
    
    
    juce::Label levelLabel;
    juce::Label levelTextLabel;
    juce::Label feedbackLabel;
    juce::Label feedbackTextLabel;
    juce::Label hpTextLabel;
    juce::Label lpTextLabel;
    juce::Label hpLabel;
    juce::Label lpLabel;
    juce::Label leftLevelLabel;
    juce::Label rightLevelLabel;
    juce::Label widthLabel;
    
    juce::Label bpmLabel;
    bool inBpmMode = true;
    
    DarkLayer darkLayer;
    ThemeSettingsComponent themeComponent;
    
    HoverImageButtonLF menuIconLF;
    MenuBar menuBar;
    juce::ImageButton onOffButton;
    juce::ImageButton bpmSwitch;
    juce::Label timeLabel;
    
    juce::HyperlinkButton webLinkButton;
    
    juce::String websiteURI = "";
    juce::String appName = "";
    juce::URL websiteLink = juce::URL("");
    
    std::unique_ptr<juce::Drawable> menuIconDrawable;

    LogoutButtonLF logoutButtonLF;
    MenuLookAndFeel menuLF;
    MenuButtonIcon menuIcon;
    juce::ImageButton menuButton;
    
    juce::PopupMenu menu;
    
    juce::ComboBox dropdown;
    
    std::string inputLevelText = "...";
    
    float allTimeHighInput = 0;
    std::function<void()> logoutButtonCallback;
    std::function<void()> openPreferencesCallback;
    std::function<void()> themeCancelCallback;
    EffectBaseImage effectImage;
    
    
    float eqTransparancy = 0.0f;
    float eqMaxTransparancy = 0.9f;
    float delayTransparancy = 0.0f;
    float delayMaxTransparancy = 0.9f;
    float fadeTime = 0.15;
    float fps = 60;
    bool isLowPassKnobActive = false;
    bool isHighPassKnobActive = false;
    bool isTimeKnobActive = false;
    bool isBpmKnobActive = false;
    bool isLevelKnobActive = false;
    bool isFeedbackKnobActive = false;
    bool isWidthKnobActive = false;
    bool isLeftLevelKnobActive = false;
    bool isRightLevelKnobActive = false;
    bool delayScreenIsSelected = true;
    

public:
    
    AppGUI(D1DelayAudioProcessor& p)
    : audioProcessor (p), drawLayer(p), eqDrawing(p), delayDrawing(p), eqFadeDrawing(p), delayFadeDrawing(p), widthDrawing(p), menuBar(p)
    {
        juce::Desktop::getInstance().addGlobalMouseListener(this);
        menuBar.setLoadPresetCallback([this](DelayPreset preset) {
            loadPreset(preset);
        });
        menuBar.setAppName(audioProcessor.getProductName());
        
        setOpenPreferencesCallback(openPreferencesCallback);
        setThemeCancelCallback(themeCancelCallback);
        
        onOffButton.setButtonText("ON / OFF");
        onOffButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        onOffButton.addListener(this);
        inBpmMode = audioProcessor.getBpmMode();


        muteLeftButton.setButtonText("L");
        muteLeftButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        muteLeftButton.addListener(this);
        
        muteRightButton.setButtonText("R");
        muteRightButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        muteRightButton.addListener(this);
        
        clearButton.setButtonText("clear");
        clearButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        clearButton.addListener(this);
        
        pingPongButton.setButtonText("ping pong");
        pingPongButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        pingPongButton.addListener(this);
        
        displayEQButton.setButtonText("EQ");
        displayEQButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        displayEQButton.addListener(this);
        
        displayDelayButton.setButtonText("Delay");
        displayDelayButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        displayDelayButton.addListener(this);

        displayWidthButton.setButtonText("Width");
        displayWidthButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        displayWidthButton.addListener(this);

        bpmSelectButton.setButtonText("BPM Select");
        bpmSelectButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        bpmSelectButton.addListener(this);
        
        bpmSelectButton.setTooltip("Profile");
        bpmSelectButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        bpmSelectButton.onClick = [&]
        {
            juce::PopupMenu menu;
            menu.setLookAndFeel(&menuLF);
            menu.addItem("1/32", [this] { selectOneThirtySecond(); });
            menu.addItem("1/16", [this] { selectOneSixteenth(); });
            menu.addItem("1/8", [this] { selectOneEigth(); });
            menu.addItem("1/4", [this] { selectOneQuarter(); });
            menu.addItem("1/3", [this] { selectOneThird(); });
            menu.addItem("1/2", [this] { selectOneHalf(); });
            menu.addItem("1/1", [this] { selectOneWhole(); });

            menu.showMenuAsync(juce::PopupMenu::Options{}.withTargetComponent(bpmSelectButton));
        };
        
        menuButton.setImages(false, true, true, juce::Image(), 1.0f, {}, juce::Image(), 1.0f, {}, juce::Image(), 1.0f, {});
        menuButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        menuButton.setColour(juce::TextButton::buttonColourId, juce::Colour(20, 27, 39));
        menuButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        menuButton.onClick = [this]()
        {
            juce::PopupMenu menu;
            menu.addItem(1, "Logout");

            menu.setLookAndFeel(&menuLF);
            auto screenOrigin = getScreenPosition();
            auto menuWidth =  static_cast<int>(getWidth()*.1);
            auto menuHeight = 40;
            int screenX = screenOrigin.getX();
            int screenY = screenOrigin.getY();
            int menuX = screenX + getWidth()*.9 - menuWidth / 2;
            int menuY = screenY;
            
            
            menu.showMenuAsync(
                juce::PopupMenu::Options()
                    .withTargetScreenArea({ menuX, menuY, menuWidth, menuHeight })
                    .withMinimumWidth(menuWidth)
                    .withPreferredPopupDirection(juce::PopupMenu::Options::PopupDirection::downwards),
                [this](int result)
                {
                    if (result == 1)
                    {
                        logoutButtonCallback();
                    }
                });
        };

        
        webLinkButton.setButtonText(websiteURI);

        webLinkButton.setURL(websiteLink);
        webLinkButton.setFont(juce::FontOptions("Arial", getHeight()*.06, juce::Font::bold), false, juce::Justification::centred);
        webLinkButton.getLookAndFeel().setColour(juce::HyperlinkButton::textColourId, themeColors.color2);
        webLinkButton.addListener(this);
        
        if(audioProcessor.isOn())
        {
            effectImage.turnOn();
        }
        else
        {
            effectImage.turnOff();
        }
        
        
        levelTextLabel.setText(juce::String("Level: "), juce::NotificationType::dontSendNotification);
        levelTextLabel.setEditable(false);
        levelTextLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(220,220,220));
        levelLabel.setJustificationType(juce::Justification::centredLeft);
        
        feedbackTextLabel.setText(juce::String("Feedback: "), juce::NotificationType::dontSendNotification);
        feedbackTextLabel.setEditable(false);
        feedbackTextLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(220,220,220));
        feedbackTextLabel.setJustificationType(juce::Justification::centredLeft);
        
        lpTextLabel.setText(juce::String("LP: "), juce::NotificationType::dontSendNotification);
        lpTextLabel.setEditable(false);
        lpTextLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(220,220,220));
        lpTextLabel.setJustificationType(juce::Justification::centredLeft);
        
        hpTextLabel.setText(juce::String("HP: "), juce::NotificationType::dontSendNotification);
        hpTextLabel.setEditable(false);
        hpTextLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(220,220,220));
        hpTextLabel.setJustificationType(juce::Justification::centredLeft);
        
        
        float labelHeight = getHeight()*.10;
        levelKnob.setLookAndFeel(&levelLF);
        levelLF.setMinKnobValue(0);
        levelLF.setMaxKnobValue(10);
        levelKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        levelKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, labelHeight * 3, labelHeight);
        levelKnob.setRange(0, 10);
        levelKnob.setValue(audioProcessor.getLevel() * 10);
        levelLF.updateImage(levelKnob.getValue());
        levelKnob.addListener(this);
        levelKnob.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        
        
        levelLabel.setText(juce::String(timeKnob.getValue()), juce::NotificationType::dontSendNotification);
        levelLabel.setEditable(true);
        levelLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(220,220,220));
        levelLabel.setJustificationType(juce::Justification::centredLeft);
        levelLabel.onTextChange = [this] { updateLevelFromLabel(); };
        levelLabel.setLookAndFeel(&textBoxLF);
        
        highPassKnob.setLookAndFeel(&highPassLF);
        highPassKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        highPassKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, labelHeight * 3, labelHeight);
        highPassKnob.setRange(20, 20000, 1);
        highPassKnob.setSkewFactorFromMidPoint(500.f);
        highPassLF.setMinKnobValue(0);
        highPassLF.setMaxKnobValue(10);
        highPassKnob.setValue(audioProcessor.getHighPass());
        highPassLF.updateImage(frequencyToNormalized(highPassKnob.getValue()));
        highPassKnob.addListener(this);
        highPassKnob.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        
        hpLabel.setText(juce::String(highPassKnob.getValue()), juce::NotificationType::dontSendNotification);
        hpLabel.setEditable(true);
        hpLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(220,220,220));
        hpLabel.setJustificationType(juce::Justification::centredLeft);
        hpLabel.onTextChange = [this] { updateHpFromLabel(); };
        hpLabel.setLookAndFeel(&textBoxLF);
        
        lowPassKnob.setLookAndFeel(&lowPassLF);
        lowPassKnob.setRange(20, 20000, 1);
        lowPassLF.setMinKnobValue(0);
        lowPassLF.setMaxKnobValue(10);
        lowPassKnob.setSkewFactorFromMidPoint(500.0);
        lowPassKnob.setValue(audioProcessor.getLowPass());
        lowPassLF.updateImage(frequencyToNormalized(lowPassKnob.getValue()));
        lowPassKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        lowPassKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, labelHeight * 3, labelHeight);
        lowPassKnob.addListener(this);
        lowPassKnob.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        
        lpLabel.setText(juce::String(lowPassKnob.getValue()), juce::NotificationType::dontSendNotification);
        lpLabel.setEditable(true);
        lpLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(220,220,220));
        lpLabel.setJustificationType(juce::Justification::centredLeft);
        lpLabel.onTextChange = [this] { updateLpFromLabel(); };
        lpLabel.setLookAndFeel(&textBoxLF);
    
        leftLevelKnob.setLookAndFeel(&leftLevelKnobLF);
        leftLevelKnobLF.setMinKnobValue(0);
        leftLevelKnobLF.setMaxKnobValue(10);
        leftLevelKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        leftLevelKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, labelHeight * 3, labelHeight);
        leftLevelKnob.setRange(0, 10,.01f);
        leftLevelKnob.setValue(audioProcessor.getLeftLevel() * 10);
        leftLevelKnob.addListener(this);
        leftLevelKnob.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        leftLevelKnobLF.updateImage(leftLevelKnob.getValue());
        
        leftLevelLabel.setText(juce::String(leftLevelKnob.getValue()), juce::NotificationType::dontSendNotification);
        leftLevelLabel.setEditable(true);
        leftLevelLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(220,220,220));
        leftLevelLabel.setJustificationType(juce::Justification::left);
        leftLevelLabel.onTextChange = [this] { updateLeftLevelFromLabel(); };
        leftLevelLabel.setLookAndFeel(&textBoxLF);
        
        rightLevelKnob.setLookAndFeel(&rightLevelKnobLF);
        rightLevelKnobLF.setMinKnobValue(0);
        rightLevelKnobLF.setMaxKnobValue(10);
        rightLevelKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        rightLevelKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, labelHeight * 3, labelHeight);
        rightLevelKnob.setRange(0, 10, .01f);
        rightLevelKnob.setValue(audioProcessor.getRightLevel() * 10);
        rightLevelKnob.addListener(this);
        rightLevelKnob.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        rightLevelKnobLF.updateImage(rightLevelKnob.getValue());
        
        rightLevelLabel.setText(juce::String(rightLevelKnob.getValue()), juce::NotificationType::dontSendNotification);
        rightLevelLabel.setEditable(true);
        rightLevelLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(220,220,220));
        rightLevelLabel.setJustificationType(juce::Justification::left);
        rightLevelLabel.onTextChange = [this] { updateRightLevelFromLabel(); };
        rightLevelLabel.setLookAndFeel(&textBoxLF);
        
        widthKnob.setLookAndFeel(&widthKnobLF);
        widthKnobLF.setMinKnobValue(0);
        widthKnobLF.setMaxKnobValue(10);
        widthKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        widthKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, labelHeight * 3, labelHeight);
        widthKnob.setRange(0, 10, .01);
        widthKnob.setValue(audioProcessor.getWidth() * 10);
        widthKnobLF.updateImage(audioProcessor.getWidth() * 10);
        widthKnob.addListener(this);
        widthKnob.setMouseCursor(juce::MouseCursor::PointingHandCursor);

        widthLabel.setText(juce::String(widthKnob.getValue()), juce::NotificationType::dontSendNotification);
        widthLabel.setEditable(true);
        widthLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(220,220,220));
        widthLabel.setJustificationType(juce::Justification::left);
        widthLabel.onTextChange = [this] { updateWidthFromLabel(); };
        widthLabel.setLookAndFeel(&textBoxLF);
        
        feedbackKnob.setLookAndFeel(&feedbackLF);
        feedbackLF.setMinKnobValue(0);
        feedbackLF.setMaxKnobValue(10);
        feedbackKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        feedbackKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, labelHeight * 3, labelHeight);
        feedbackKnob.setRange(0, 10, .1);
        feedbackKnob.setValue(audioProcessor.getFeedback() * 10);
        feedbackLF.updateImage(feedbackKnob.getValue());
        feedbackKnob.addListener(this);
        feedbackKnob.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        
        feedbackLabel.setText(juce::String(feedbackKnob.getValue()), juce::NotificationType::dontSendNotification);
        feedbackLabel.setEditable(true);
        feedbackLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(220,220,220));
        feedbackLabel.setJustificationType(juce::Justification::left);
        feedbackLabel.onTextChange = [this] { updateFeedbackFromLabel(); };
        feedbackLabel.setLookAndFeel(&textBoxLF);
        
        timeKnob.setLookAndFeel(&timeLF);
        timeLF.setMinKnobValue(1);
        timeLF.setMaxKnobValue(1000.f);
        timeLF.setLabelEnding(" ms");
        timeKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        timeKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, labelHeight * 3, labelHeight);
        timeKnob.setRange(1, 1000.f, 1);
        timeKnob.setValue(audioProcessor.getTime() * 1000.f);
        timeLF.updateImage(timeKnob.getValue());
        timeKnob.addListener(this);
        timeKnob.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        
        timeLabel.setText(juce::String(timeKnob.getValue()) + " ms", juce::NotificationType::dontSendNotification);
        timeLabel.setEditable(true);
        timeLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(220,220,220));
        timeLabel.setJustificationType(juce::Justification::left);
        timeLabel.onTextChange = [this] { updateTimeFromLabel(); };
        timeLabel.setLookAndFeel(&textBoxLF);
        bpmKnob.setLookAndFeel(&bpmLF);
        bpmLF.setMinKnobValue(0);
        bpmLF.setMaxKnobValue(6);
        bpmLF.setLabelEnding("");
        bpmKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        bpmKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        bpmKnob.setRange(0, 6, 1);
        bpmKnob.setValue(audioProcessor.getBPMSelection());
        bpmLF.updateImage(audioProcessor.getBPMSelection());
        bpmKnob.addListener(this);
        bpmKnob.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        
        bpmButton.setButtonText("bpm");
        bpmButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        bpmButton.addListener(this);
        
        msButton.setButtonText("ms");
        msButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        msButton.addListener(this);
        
        widthDrawing.setWidth(audioProcessor.getWidth());
        
        addAndMakeVisible(dummyFocusCatcher);
        dummyFocusCatcher.setBounds(0, 0, 1, 1);
        dummyFocusCatcher.setInterceptsMouseClicks(false, false); // Doesn't block clicks
        dummyFocusCatcher.setWantsKeyboardFocus(true);
        levelKnob.setWantsKeyboardFocus(true);
        feedbackKnob.setWantsKeyboardFocus(true);

        juce::Desktop::getInstance().addGlobalMouseListener(this);
        
        
        addAndMakeVisible(&floor);
        addAndMakeVisible(&effectImage);
        addAndMakeVisible(&drawLayer);
        addAndMakeVisible(&eqDrawing);
        addAndMakeVisible(&delayDrawing);
        addAndMakeVisible(&eqFadeDrawing);
        addAndMakeVisible(&delayFadeDrawing);
        addAndMakeVisible(&widthDrawing);
        addAndMakeVisible(&onOffSwitchImage);
        addAndMakeVisible(&levelKnob);
        addAndMakeVisible(&lowPassKnob);
        addAndMakeVisible(&highPassKnob);
        addAndMakeVisible(&leftLevelKnob);
        addAndMakeVisible(&rightLevelKnob);
        addAndMakeVisible(&widthKnob);
        addAndMakeVisible(&onOffButton);
        addAndMakeVisible(&webLinkButton);
        addAndMakeVisible(&menuIcon);
        addAndMakeVisible(&menuButton);
        addAndMakeVisible(&bpmButton);
        addAndMakeVisible(&msButton);
        addAndMakeVisible(&muteLeftButton);
        addAndMakeVisible(&muteRightButton);
        addAndMakeVisible(&clearButton);
        addAndMakeVisible(&pingPongButton);
        addAndMakeVisible(&displayEQButton);
        addAndMakeVisible(&displayDelayButton);
        addAndMakeVisible(&displayWidthButton);
        addAndMakeVisible(&timeKnob);
        addAndMakeVisible(&bpmKnob);
        addAndMakeVisible(&feedbackKnob);
        addAndMakeVisible(&darkLayer);
        darkLayer.setVisible(false);
        addAndMakeVisible(&themeComponent);
        themeComponent.setVisible(false);
        
        timeTextBox.addListener(this);
        
        addAndMakeVisible(&timeLabel);
        addAndMakeVisible(&levelTextLabel);
        addAndMakeVisible(&feedbackTextLabel);
        addAndMakeVisible(&hpTextLabel);
        addAndMakeVisible(&lpTextLabel);
        addAndMakeVisible(&levelLabel);
        addAndMakeVisible(&feedbackLabel);
        addAndMakeVisible(&lpLabel);
        addAndMakeVisible(&hpLabel);
        addAndMakeVisible(&leftLevelLabel);
        addAndMakeVisible(&rightLevelLabel);
        addAndMakeVisible(&widthLabel);
        addAndMakeVisible(&bpmSelectButton);
        
        float width = getWidth();
        addAndMakeVisible(&timeTextBox);
        addAndMakeVisible(&menuBar);
        
        eqFadeDrawing.setAlpha(eqTransparancy);
        delayFadeDrawing.setAlpha(delayTransparancy);
        
        if(audioProcessor.getBpmMode())
        {
            bpmKnob.setVisible(true);
            timeLabel.setVisible(false);
            bpmSelectButton.setVisible(true);
            timeKnob.setVisible(false);
        }
        else
        {
            bpmKnob.setVisible(false);
            timeLabel.setVisible(true);
            bpmSelectButton.setVisible(false);
            timeKnob.setVisible(true);
        }
        
        if(!audioProcessor.isOn())
        {
            delayDrawing.setVisible(false);
            widthDrawing.setVisible(false);
            levelLabel.setVisible(false);
            levelTextLabel.setVisible(false);
            feedbackLabel.setVisible(false);
            hpTextLabel.setVisible(false);
            lpTextLabel.setVisible(false);
            feedbackTextLabel.setVisible(false);
            timeLabel.setVisible(false);
            bpmLabel.setVisible(false);
            hpLabel.setVisible(false);
            lpLabel.setVisible(false);
            rightLevelLabel.setVisible(false);
            leftLevelLabel.setVisible(false);
            widthLabel.setVisible(false);
            
            
        }
        eqDrawing.setVisible(false);
    }
    
    ~AppGUI()
    {
        juce::Desktop::getInstance().removeGlobalMouseListener(this);
    }
    
    void setAppName(juce::String appName)
    {
        this->appName = appName;
        menuBar.setAppName(appName);
    }
    
    void paint(juce::Graphics& g) override
    {
        
    }
    
    void resized() override
    {
        //menuLF.setFontSize(getHeight()*.0275);
        float width = getWidth();
        float height = getHeight();
        textHeight = height * .06;
        
        float knobSize = width*.065;
        floor.setBounds(0,0,width,height);
        dummyFocusCatcher.setBounds(0, 0, 1, 1);
        effectImage.setBounds(0,0,width,height);
        drawLayer.setBounds(0,0,width,height);
        delayDrawing.setBounds(width*.23f, height*.38f, width * .54, height * .32);
        eqDrawing.setBounds(width*.23f, height*.38f, width * .54, height * .32);
        eqFadeDrawing.setBounds(width*.23f, height*.38f, width * .54, height * .32);
        delayFadeDrawing.setBounds(width*.23f, height*.38f, width * .54, height * .32);
        widthDrawing.setBounds(width*.23f, height*.72, width*.54, height*.04);
        levelKnob.setBounds(width*.0725, height*.23, knobSize, knobSize);
        leftLevelKnob.setBounds(width*.795 , height*.60, knobSize, knobSize);
        rightLevelKnob.setBounds(width * .863, height*.60, knobSize, knobSize);
        
        timeKnob.setBounds(width*.0725, height*.46, knobSize, knobSize);
        bpmKnob.setBounds(width*.0725, height*.46, knobSize, knobSize);
        feedbackKnob.setBounds(width*.14, height*.23, knobSize, knobSize);
        
        lowPassKnob.setBounds(width*.14,  height*.69, knobSize, knobSize);
        highPassKnob.setBounds(width*.0725,  height*.69, knobSize, knobSize);
        widthKnob.setBounds(width*.14, getHeight()*.46, knobSize, knobSize);
        
        webLinkButton.setBounds(width*.5 - width *.07, height*.93, width*.14,height*.04);
        webLinkButton.setFont(juce::FontOptions("Arial", height*.03, juce::Font::bold), false, juce::Justification::centredTop);
        menuButton.setBounds(width*.95, height*.02, width*.03, width*.03); // Adjust size as needed
        menuIcon.setBounds(width*.95, height*.02, width*.03, width*.03);
        onOffButton.setBounds(width*.82, height*.205, width*.09, height*.085);
        bpmButton.setBounds(width*.805, height*.333,width*.055, height*.055);
        msButton.setBounds(width*.87, height*.333,width*.055, height*.055);
        clearButton.setBounds(width*.805, height*.405,width*.055, height*.055);
        pingPongButton.setBounds(width*.87, height*.405,width*.055, height*.055);
        muteLeftButton.setBounds(width*.805, height*.48,width*.055, height*.055);
        muteRightButton.setBounds(width*.87, height*.48,width*.055, height*.055);
        displayDelayButton.setBounds(width*.333, height * .812, width*.054, height * .054);
        displayEQButton.setBounds(width*.398, height * .812, width*.054, height * .054);
        darkLayer.setBounds(0,0,width,height);
        themeComponent.setBounds(100,100,300,400);
        
        feedbackTextLabel.setFont(juce::FontOptions(height*.04));
        levelTextLabel.setFont(juce::FontOptions(height*.04));
        hpTextLabel.setFont(juce::FontOptions(height*.03));
        lpTextLabel.setFont(juce::FontOptions(height*.03));
        feedbackTextLabel.setBounds(width*.23, height*.283, width*.1, height*.04);
        levelTextLabel.setBounds(width*.23, height*.233, width*.1, height*.04);
        hpTextLabel.setBounds(width*.23, height*.331, width*.06, height*.03);
        lpTextLabel.setBounds(width*.325, height*.331, width*.06, height*.03);
        
        
        levelLabel.setFont(juce::FontOptions(height*.04));
        levelLabel.setBounds(width*.28,height*.233, width*.07, height*.04);
        timeLabel.setFont(juce::FontOptions(height*.04));
        feedbackLabel.setFont(juce::FontOptions(height*.04));
        hpLabel.setFont(juce::FontOptions(height*.03));
        lpLabel.setFont(juce::FontOptions(height*.03));
        leftLevelLabel.setFont(juce::FontOptions(height*.03));
        rightLevelLabel.setFont(juce::FontOptions(height*.03));
        widthLabel.setFont(juce::FontOptions(height*.03));
        
        feedbackLabel.setBounds(width*.315,height*.283, width*.07,height*.04);
        hpLabel.setBounds(width*.2535, height*.331, width*.08,height*.03);
        lpLabel.setBounds(width*.345,height*.331, width*.08,height*.03);
        leftLevelLabel.setBounds(width * 0.62f, height * 0.33f, width*.07,height*.03);
        rightLevelLabel.setBounds(width*.705, height*.33, width*.07, height*.03);
        widthLabel.setBounds(width*.51,height*.755, width*.07,height*.04);
        
        timeLabel.setBounds(width*.645,height*.28, width * .08f, height*.04);
        bpmSelectButton.setBounds(width*.66,height*.263, width * .06f, height*.07);
        menuBar.setBounds(0,0,width,height*.08);
        
        
        repaint();

    }
    
    
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override
    {
        if (&editor == &timeTextBox)
            updateTimeFromTextBox();
            
    }
    
    void textEditorFocusLost(juce::TextEditor& editor) override
    {
        if (&editor == &timeTextBox)
            updateTimeFromTextBox();
            
    }
    
    void mouseDown(const juce::MouseEvent& event) override
    {
        auto clickPos = event.getScreenPosition().roundToInt();

        // Check if the click was outside the textbox
        if (!timeTextBox.getScreenBounds().contains(clickPos))
        {
            if (timeTextBox.hasKeyboardFocus(true))
            {
                updateTimeFromTextBox();
                dummyFocusCatcher.grabKeyboardFocus();
            }
        }
    }
    
    void selectOneThirtySecond()
    {
        int selection = 0;
        audioProcessor.setTimeBpm(selection);
        bpmKnob.setValue(selection);
        bpmLF.updateImage(selection);
        delayDrawing.updateData();
        bpmLabel.setText("1/32",  juce::NotificationType::dontSendNotification);
    }
    
    void selectOneSixteenth()
    {
        int selection = 1;
        audioProcessor.setTimeBpm(selection);
        bpmKnob.setValue(selection);
        bpmLF.updateImage(selection);
        delayDrawing.updateData();
        bpmLabel.setText("1/16",  juce::NotificationType::dontSendNotification);
    }
    
    void selectOneEigth()
    {
        int selection = 2;
        audioProcessor.setTimeBpm(selection);
        bpmKnob.setValue(selection);
        bpmLF.updateImage(selection);
        delayDrawing.updateData();
        bpmLabel.setText("1/8",  juce::NotificationType::dontSendNotification);
    }
    
    void selectOneQuarter()
    {
        int selection = 3;
        audioProcessor.setTimeBpm(selection);
        bpmKnob.setValue(selection);
        bpmLF.updateImage(selection);
        delayDrawing.updateData();
        bpmLabel.setText("1/4",  juce::NotificationType::dontSendNotification);
    }
    
    void selectOneThird()
    {
        int selection = 4;
        audioProcessor.setTimeBpm(selection);
        bpmKnob.setValue(selection);
        bpmLF.updateImage(selection);
        delayDrawing.updateData();
        bpmLabel.setText("1/3",  juce::NotificationType::dontSendNotification);
    }
    
    void selectOneHalf()
    {
        int selection = 5;
        audioProcessor.setTimeBpm(selection);
        bpmKnob.setValue(selection);
        bpmLF.updateImage(selection);
        delayDrawing.updateData();
        bpmLabel.setText("1/2",  juce::NotificationType::dontSendNotification);
    }
    
    void selectOneWhole()
    {
        int selection = 6;
        audioProcessor.setTimeBpm(selection);
        bpmKnob.setValue(selection);
        bpmLF.updateImage(selection);
        delayDrawing.updateData();
        bpmLabel.setText("1/1",  juce::NotificationType::dontSendNotification);
    }
    
    
    void updateTimeFromLabel()
    {
        auto text = timeLabel.getText().trim();
        auto newValue = text.getFloatValue();

        // Clamp between 1 and 1000 milliseconds
        newValue = std::max(1.0f, std::min(newValue, 1000.0f));

        audioProcessor.setTime(newValue / 1000.0f);
        timeKnob.setValue(newValue);
        timeLF.updateImage(newValue);
        delayDrawing.updateData();
        repaint();
    }
    
    void updateLevelFromLabel()
    {
        auto text = levelLabel.getText().trim();
        auto newValue = text.getFloatValue();
        newValue = std::max(0.0f, std::min(newValue, 10.f));

        audioProcessor.setLevel(newValue / 10);
        levelKnob.setValue(newValue);
        levelLF.updateImage(newValue);
        delayDrawing.updateData();
        repaint();
    }
    
    void updateFeedbackFromLabel()
    {
        auto text = feedbackLabel.getText().trim();
        auto newValue = text.getFloatValue();
        newValue = std::max(0.0f, std::min(newValue, 10.f));

        audioProcessor.setFeedback(newValue / 10);
        feedbackKnob.setValue(newValue);
        feedbackLF.updateImage(newValue);
        delayDrawing.updateData();
        repaint();
    }
    
    void updateHpFromLabel()
    {
        auto text = hpLabel.getText().trim();
        auto newValue = text.getFloatValue();
        newValue = std::max(0.0f, std::min(newValue, 20000.f));

        audioProcessor.setHighPass(newValue);
        highPassKnob.setValue(newValue);
        highPassLF.updateImage(newValue);
        delayDrawing.updateData();
        repaint();
    }
    
    void updateLpFromLabel()
    {
        auto text = lpLabel.getText().trim();
        auto newValue = text.getFloatValue();
        newValue = std::max(0.0f, std::min(newValue, 20000.f));

        audioProcessor.setLowPass(newValue);
        lowPassKnob.setValue(newValue);
        lowPassLF.updateImage(newValue);
        delayDrawing.updateData();
        repaint();
    }
    
    void updateLeftLevelFromLabel()
    {
        auto text = leftLevelLabel.getText().trim();
        auto newValue = text.getFloatValue();
        newValue = std::max(0.0f, std::min(newValue, 100.f));

        audioProcessor.setLeftLevel(newValue/100.0f);
        leftLevelKnob.setValue(newValue/10.f);
        leftLevelKnobLF.updateImage(newValue/10.f);
        delayDrawing.updateData();
        repaint();
    }
    
    void updateRightLevelFromLabel()
    {
        auto text = rightLevelLabel.getText().trim();
        auto newValue = text.getFloatValue();
        newValue = std::max(0.0f, std::min(newValue, 100.f));

        audioProcessor.setRightLevel(newValue/100.f);
        rightLevelKnob.setValue(newValue/10.f);
        rightLevelKnobLF.updateImage(newValue/10.f);
        rightLevelLabel.setText(juce::String(rightLevelLabel.getText().trim()) + " %", juce::NotificationType::dontSendNotification);
        delayDrawing.updateData();
        repaint();
    }
    
    void updateWidthFromLabel()
    {
        auto text = widthLabel.getText().trim();
        auto newValue = text.getFloatValue();
        newValue = std::max(0.0f, std::min(newValue, 100.f));

        audioProcessor.setWidth(newValue/100.f);
        widthKnob.setValue(newValue/10.f);
        widthKnobLF.updateImage(newValue/10.f);
        delayDrawing.updateData();
        repaint();
    }
    
    void updateTimeFromTextBox()
    {
        auto inputText = timeTextBox.getText().trim();
        auto newValue = inputText.getFloatValue();

        // Clamp to range (1ms to 1000ms)
        newValue = std::max(1.0f, std::min(newValue, 1000.0f));

        audioProcessor.setTime(newValue / 1000.f);
        timeKnob.setValue(newValue);
        timeLF.updateImage(newValue);
        delayDrawing.updateData();
        repaint();
    }
    
    void sliderDragStarted(juce::Slider* slider) override
    {
            
        if (slider == &levelKnob)
        {
            isLevelKnobActive = true;
        }
        if (slider == &timeKnob)
        {
            isTimeKnobActive = true;
        }
        if (slider == &bpmKnob)
        {
            isBpmKnobActive = true;
        }
        if (slider == &feedbackKnob)
        {
            isFeedbackKnobActive = true;
        }
        if (slider == &widthKnob)
        {
            isWidthKnobActive = true;
        }
        if (slider == &leftLevelKnob)
        {
            isLeftLevelKnobActive = true;
        }
        if (slider == &rightLevelKnob)
        {
            isRightLevelKnobActive = true;
        }
        if (slider == &lowPassKnob)
        {
            isLowPassKnobActive = true;
        }
        
        if (slider == &highPassKnob)
        {
            isHighPassKnobActive = true;
        }
    };
    
    
    void sliderDragEnded(juce::Slider* slider) override
    {
        if (slider == &levelKnob)
        {
            isLevelKnobActive = false;
        }
        if (slider == &timeKnob)
        {
            isTimeKnobActive = false;
        }
        if (slider == &bpmKnob)
        {
            isBpmKnobActive = false;
        }
        if (slider == &feedbackKnob)
        {
            isFeedbackKnobActive = false;
        }
        if (slider == &widthKnob)
        {
            isWidthKnobActive = false;
        }
        if (slider == &leftLevelKnob)
        {
            isLeftLevelKnobActive = false;
        }
        if (slider == &rightLevelKnob)
        {
            isRightLevelKnobActive = false;
        }
        if (slider == &lowPassKnob)
        {
            isLowPassKnobActive = false;
        }
        if (slider == &highPassKnob)
        {
            isHighPassKnobActive = false;
        }
    }
    
    
    
    void buttonClicked(juce::Button* button) override
    {

        if (button == &onOffButton)
        {
            

            if(audioProcessor.isOn())
            {
                audioProcessor.turnOff();
                delayDrawing.updateData();
                drawLayer.setDisplayEq(false);
                drawLayer.setDisplayDelay(true);
                effectImage.turnOff();
                onOffSwitchImage.turnOff();
                delayDrawing.setVisible(false);
                delayFadeDrawing.setVisible(false);
                eqDrawing.setVisible(false);
                eqFadeDrawing.setVisible(false);
                widthDrawing.setVisible(false);
                delayScreenIsSelected = true;
                levelLabel.setVisible(false);
                levelTextLabel.setVisible(false);
                feedbackLabel.setVisible(false);
                feedbackTextLabel.setVisible(false);
                timeLabel.setVisible(false);
                bpmSelectButton.setVisible(false);
                hpLabel.setVisible(false);
                lpLabel.setVisible(false);
                hpTextLabel.setVisible(false);
                lpTextLabel.setVisible(false);
                rightLevelLabel.setVisible(false);
                leftLevelLabel.setVisible(false);
                widthLabel.setVisible(false);
                repaint();
            }
            else
            {
                audioProcessor.turnOn();
                delayDrawing.updateData();
                drawLayer.setDisplayEq(false);
                drawLayer.setDisplayDelay(true);
                effectImage.turnOn();
                onOffSwitchImage.turnOn();
                delayDrawing.setVisible(true);
                delayFadeDrawing.setVisible(true);
                eqDrawing.setVisible(true);
                eqFadeDrawing.setVisible(true);
                widthDrawing.setVisible(true);
                delayScreenIsSelected = true;
                levelLabel.setVisible(true);
                levelTextLabel.setVisible(true);
                feedbackLabel.setVisible(true);
                feedbackTextLabel.setVisible(true);
                hpTextLabel.setVisible(true);
                lpTextLabel.setVisible(true);
                bpmSelectButton.setVisible(true);
                hpLabel.setVisible(true);
                lpLabel.setVisible(true);
                rightLevelLabel.setVisible(true);
                leftLevelLabel.setVisible(true);
                widthLabel.setVisible(true);
                repaint();

                if (inBpmMode)
                {
                    bpmLabel.setVisible(true);
                    timeLabel.setVisible(false);
                }
                else
                {
                    bpmLabel.setVisible(false);
                    timeLabel.setVisible(true);
                }
            }
        }
        if (button == &bpmButton)
        {
                inBpmMode = true;
                audioProcessor.setBpmMode(true);
                int selection = audioProcessor.getBPMSelection();
            switch (selection)
            {
                case 0 : bpmLabel.setText("1/32",  juce::NotificationType::dontSendNotification); return;
                case 1 : bpmLabel.setText("1/16",  juce::NotificationType::dontSendNotification); return;
                case 2 : bpmLabel.setText("1/8",  juce::NotificationType::dontSendNotification); return;
                case 3 : bpmLabel.setText("1/4",  juce::NotificationType::dontSendNotification); return;
                case 4 : bpmLabel.setText("1/3",  juce::NotificationType::dontSendNotification); return;
                case 5 : bpmLabel.setText("1/2",  juce::NotificationType::dontSendNotification); return;
                case 6 : bpmLabel.setText("1/1",  juce::NotificationType::dontSendNotification); return;
            }
            
                bpmLabel.setJustificationType(4);
                bpmKnob.setVisible(true);
                bpmLabel.setVisible(true);
                timeKnob.setVisible(false);
                timeLabel.setVisible(false);
                bpmSelectButton.setVisible(true);
                delayDrawing.updateData();
                repaint();
                    
        }
        
        
        if (button == &msButton)
        {

            inBpmMode = false;
            audioProcessor.setBpmMode(false);
            bpmLabel.setJustificationType(4);
            bpmKnob.setVisible(false);
            bpmLabel.setVisible(false);
            timeKnob.setVisible(true);
            timeLabel.setVisible(true);
            bpmSelectButton.setVisible(false);
            delayDrawing.updateData();
            repaint();
                    
        }
        
        if (button == &muteLeftButton)
        {
            audioProcessor.toggleLeftMuted();
            delayDrawing.updateData();
            repaint();
        }
        if (button == &muteRightButton)
        {
            audioProcessor.toggleRightMuted();
            delayDrawing.updateData();
            repaint();
        }
        
        if (button == &clearButton)
        {
            audioProcessor.clearDelay();
            delayDrawing.updateData();
            repaint();
        }
        
        if (button == &pingPongButton)
        {
            audioProcessor.togglePingPong();
            delayDrawing.updateData();
            repaint();
        }
        
        if (button == &displayDelayButton)
        {
            delayScreenIsSelected = true;
            eqDrawing.setVisible(false);
            delayDrawing.setVisible(true);
            drawLayer.setDisplayDelay(true);
            drawLayer.setDisplayEq(false);
            delayDrawing.updateData();
        }
        
        if (button == &displayEQButton)
        {
            delayScreenIsSelected =false;
            delayDrawing.setVisible(false);
            eqDrawing.setVisible(true);
            drawLayer.setDisplayDelay(false);
            drawLayer.setDisplayEq(true);
            delayDrawing.updateData();
        }
        
        if (button == &displayWidthButton)
        {
            delayDrawing.updateData();
        }

    }
    void sliderValueChanged(juce::Slider* slider) override
    {
        
        if (slider == &levelKnob)
        {
            
            audioProcessor.setLevel(levelKnob.getValue()/10);
            levelLF.updateImage(levelKnob.getValue());
            levelLabel.setText(juce::String(levelKnob.getValue(),1), juce::NotificationType::dontSendNotification);
            delayDrawing.updateData();
            repaint();
        }
        if (slider == &highPassKnob)
        {
            
            audioProcessor.setHighPass(highPassKnob.getValue());
            highPassLF.updateImage(frequencyToNormalized(highPassKnob.getValue()));
            hpLabel.setText(juce::String(highPassKnob.getValue()) + " hz", juce::NotificationType::dontSendNotification);
            delayDrawing.updateData();
            repaint();

        }
        if (slider == &lowPassKnob)
        {
            
            audioProcessor.setLowPass(lowPassKnob.getValue());
            lowPassLF.updateImage(frequencyToNormalized(lowPassKnob.getValue()));
            lpLabel.setText(juce::String(lowPassKnob.getValue()) + " hz", juce::NotificationType::dontSendNotification);
            delayDrawing.updateData();
            repaint();
        }
        if (slider == &leftLevelKnob)
        {
                
            audioProcessor.setLeftLevel(leftLevelKnob.getValue()/10.f);
            leftLevelKnobLF.updateImage(leftLevelKnob.getValue());
            leftLevelLabel.setText(juce::String(leftLevelKnob.getValue() * 10.f,1) + " %", juce::NotificationType::dontSendNotification);
            delayDrawing.updateData();
            repaint();
        }
        if (slider == &rightLevelKnob)
        {
            audioProcessor.setRightLevel(rightLevelKnob.getValue()/10.f);
            rightLevelKnobLF.updateImage(rightLevelKnob.getValue());
            rightLevelLabel.setText(juce::String(rightLevelKnob.getValue() * 10.f,1) + " %", juce::NotificationType::dontSendNotification);
            delayDrawing.updateData();
            repaint();
        }
        
        if (slider == &widthKnob)
        {
            audioProcessor.setWidth(widthKnob.getValue()/10.f);
            widthKnobLF.updateImage(widthKnob.getValue());
            widthLabel.setText(juce::String(widthKnob.getValue() * 10.f,1) + " %", juce::NotificationType::dontSendNotification);
            delayDrawing.updateData();
            widthDrawing.setWidth(widthKnob.getValue()/10.f);
            repaint();
        }
        
        if (slider == &feedbackKnob)
        {
            audioProcessor.setFeedback(feedbackKnob.getValue()/10.f);
            feedbackLF.updateImage(feedbackKnob.getValue());
            feedbackLabel.setText(juce::String(feedbackKnob.getValue()), juce::NotificationType::dontSendNotification);
            delayDrawing.updateData();
            repaint();
        }
        if (slider == &timeKnob)
        {
            
            float roundedValue = std::round(timeKnob.getValue());
            audioProcessor.setTime(timeKnob.getValue()/1000.f);
            timeLF.updateImage(roundedValue);
            delayDrawing.updateData();
            float curTimeMS = timeKnob.getValue();
            timeLabel.setText(juce::String(curTimeMS) + " ms", juce::NotificationType::dontSendNotification);
            
            repaint();
        }
        
        if(slider == &bpmKnob)
        {
            audioProcessor.setTimeBpm(bpmKnob.getValue());
            bpmLF.updateImage(bpmKnob.getValue());
            delayDrawing.updateData();
            int selection = audioProcessor.getBPMSelection();
            if(selection == 0)
            {
                bpmLabel.setText("1/32",  juce::NotificationType::dontSendNotification);
            }
            if(selection == 1)
            {
                bpmLabel.setText("1/16",  juce::NotificationType::dontSendNotification);
            }
            if(selection == 2)
            {
                bpmLabel.setText("1/8",  juce::NotificationType::dontSendNotification);
            }
            if(selection == 3)
            {
                bpmLabel.setText("1/4",  juce::NotificationType::dontSendNotification);
            }
            if(selection == 4)
            {
                bpmLabel.setText("1/3",  juce::NotificationType::dontSendNotification);
            }
            if(selection == 5)
            {
                bpmLabel.setText("1/2",  juce::NotificationType::dontSendNotification);
            }
            if(selection == 6)
            {
                bpmLabel.setText("1/1",  juce::NotificationType::dontSendNotification);
            }
            
            
        }
        repaint();
    }

    float frequencyToNormalized(float freq)
    {
        float output = 0;
        if (freq <= 500.0f)
        {
            float minFreq = 20.0f;
            float maxFreq = 500.0f;
            float norm = std::log(freq / minFreq) / std::log(maxFreq / minFreq);
            output = norm * 0.5f * 10.f;
        }
        else
        {
            float minFreq = 500.0f;
            float maxFreq = 20000.0f;
            float norm = std::log(freq / minFreq) / std::log(maxFreq / minFreq);
            output = (0.5f + norm * 0.5f) * 10.f;
        }
        
        return(output);
    }
    
    void frameUpdate()
    {
        
        
        if(delayScreenIsSelected)
        {
            if(eqTransparancy  && !isLowPassKnobActive && !isHighPassKnobActive> 0)
            {
                eqTransparancy = eqTransparancy - 1/(fps * fadeTime);
            }
            if(eqTransparancy <= 0)
            {
                eqTransparancy = 0;
            }
            if(isLowPassKnobActive)
            {
                eqTransparancy = eqTransparancy + 1/(fps * fadeTime);
            }
            if(isHighPassKnobActive)
            {
                eqTransparancy = eqTransparancy + 1/(fps * fadeTime);
            }
            
            if(eqTransparancy > eqMaxTransparancy)
            {
                eqTransparancy = eqMaxTransparancy;
            }
            eqFadeDrawing.setAlpha(eqTransparancy);
        }
        else
        {
            if(delayTransparancy  && !isLevelKnobActive && !isFeedbackKnobActive && !isTimeKnobActive && !isBpmKnobActive && !isLeftLevelKnobActive && !isRightLevelKnobActive > 0)
            {
                delayTransparancy = delayTransparancy - 1/(fps * fadeTime);
            }
            if(delayTransparancy <= 0)
            {
                delayTransparancy = 0;
            }
            if(isLevelKnobActive)
            {
                delayTransparancy = delayTransparancy + 1/(fps * fadeTime);
            }
            if(isFeedbackKnobActive)
            {
                delayTransparancy = delayTransparancy + 1/(fps * fadeTime);
            }
            if(isLeftLevelKnobActive)
            {
                delayTransparancy = delayTransparancy + 1/(fps * fadeTime);
            }
            
            if(isRightLevelKnobActive)
            {
                delayTransparancy = delayTransparancy + 1/(fps * fadeTime);
            }
            if(isTimeKnobActive)
            {
                delayTransparancy = delayTransparancy + 1/(fps * fadeTime);
            }
            if(isBpmKnobActive)
            {
                delayTransparancy = delayTransparancy + 1/(fps * fadeTime);
            }
            
            if(delayTransparancy > delayMaxTransparancy)
            {
                delayTransparancy = delayMaxTransparancy;
            }
            delayFadeDrawing.setAlpha(delayTransparancy);
            delayFadeDrawing.updateData();
        }
        
        if(drawLayer.getUpdatedTimeSigFlag())
        {
            delayDrawing.updateData();
            drawLayer.setUpdatedTimeSigFlag(false);
        }
        
        widthDrawing.frameUpdate();
        drawLayer.frameUpdate();
        repaint();
        
    }
    
    void preferencesCallback()
    {
        darkLayer.setVisible(true);
        themeComponent.setVisible(true);
    }
    
    void closeThemeModal()
    {
        darkLayer.setVisible(false);
        themeComponent.setVisible(false);
    }
    
    void updateTheme(juce::Colour leftColor, juce::Colour rightColor)
    {
        delayDrawing.setLeftColor(leftColor);
        delayDrawing.setLeftColor(rightColor);
        
    }
    
    
    void setLogoutButtonCallback(std::function<void()> newCallback)
    {
        logoutButtonCallback = std::move(newCallback);
        
        menuBar.setLogoutButtonCallback([this] { logoutButtonCallback(); });
    }
    void setOpenPreferencesCallback(std::function<void()> newCallback)
    {
        openPreferencesCallback = std::move(newCallback);
        darkLayer.setVisible(true);
        menuBar.setOpenPreferencesCallback([this] { preferencesCallback(); });
    }
    
    void setThemeCancelCallback(std::function<void()> newCallback)
    {
        themeCancelCallback = std::move(newCallback);
        themeComponent.setCancelButtonCallback([this] { closeThemeModal(); });
    }
    
    void loadPreset(DelayPreset delayPreset)
    {
        const juce::ScopedLock lock(presetLock);
        float presetLevel = delayPreset.getLevel();
        float presetFdbk = delayPreset.getFeedback();
        float presetTime = delayPreset.getTime();
        float presetLeftLvl = delayPreset.getLeftLevel();
        float presetRightLvl = delayPreset.getRightLevel();
        float presetWidth = delayPreset.getWidth();
        float presetLowPass = delayPreset.getLowPass();
        float presetHighPass = delayPreset.getHighPass();
        bool presetLeftMuted = delayPreset.getLeftMuted();
        bool presetRightMuted = delayPreset.getRightMuted();
        bool presetPingPong = delayPreset.getPingPong();
        bool presetBpmMode = delayPreset.getBpmMode();
        bool presetTimeBpm = delayPreset.getTimeBpm();

        
        presetLevel = std::max(0.f, std::min(presetLevel, 10.f));
        presetFdbk = std::max(0.f, std::min(presetFdbk, 10.f));
        presetTime = std::max(0.f, std::min(presetTime, 1000.f));
        presetLeftLvl = std::max(0.f, std::min(presetLeftLvl, 10.f));
        presetRightLvl = std::max(0.f, std::min(presetRightLvl, 10.f));
        presetWidth = std::max(0.f, std::min(presetWidth, 10.f));
        presetLowPass = std::max(20.f, std::min(presetLowPass, 20000.f));
        presetHighPass = std::max(20.f, std::min(presetHighPass, 20000.f));
        
        audioProcessor.setLevel(presetLevel/10);
        audioProcessor.setFeedback(presetFdbk/10);
        if(!presetBpmMode)
        {
            audioProcessor.setTime(presetTime/1000.f);
            timeKnob.setValue(delayPreset.getTime());
        }
        audioProcessor.setLeftLevel(presetLeftLvl/10);
        audioProcessor.setRightLevel(presetRightLvl/10);
        audioProcessor.setWidth(presetWidth/10);
        audioProcessor.setLeftMuted(presetLeftMuted);
        audioProcessor.setRightMuted(presetRightMuted);
        audioProcessor.setPingPong(presetPingPong);
        audioProcessor.setBpmMode(presetBpmMode);
        audioProcessor.setTimeBpm(presetTimeBpm);
        
        audioProcessor.setLowPass(presetLowPass);
        audioProcessor.setHighPass(presetHighPass);
        
        levelKnob.setValue(presetLevel);
        highPassKnob.setValue(delayPreset.getHighPass());
        lowPassKnob.setValue(delayPreset.getLowPass());
        feedbackKnob.setValue(delayPreset.getFeedback());
        leftLevelKnob.setValue(delayPreset.getLeftLevel());
        rightLevelKnob.setValue(delayPreset.getRightLevel());
        timeLF.updateImage(delayPreset.getTime());
        if(presetBpmMode)
        {
            bpmKnob.setValue(delayPreset.getBpmKnobSelection());
            bpmLF.updateImage(delayPreset.getBpmKnobSelection());
        }
        widthKnob.setValue(delayPreset.getWidth());
        widthKnobLF.updateImage(delayPreset.getWidth());
        
        lowPassKnob.setValue(delayPreset.getLowPass());
        lowPassLF.updateImage(frequencyToNormalized(lowPassKnob.getValue()));
        highPassKnob.setValue(delayPreset.getHighPass());
        highPassLF.updateImage(frequencyToNormalized(highPassKnob.getValue()));
        
        levelLF.updateImage(levelKnob.getValue());
        leftLevelKnobLF.updateImage(leftLevelKnob.getValue());
        rightLevelKnobLF.updateImage(rightLevelKnob.getValue());
        
        audioProcessor.setTimeBpm(bpmKnob.getValue());
        bpmLF.updateImage(bpmKnob.getValue());
        delayDrawing.updateData();
        int selection = audioProcessor.getBPMSelection();
        if(selection == 0)
        {
            bpmLabel.setText("1/32",  juce::NotificationType::dontSendNotification);
        }
        if(selection == 1)
        {
            bpmLabel.setText("1/16",  juce::NotificationType::dontSendNotification);
        }
        if(selection == 2)
        {
            bpmLabel.setText("1/8",  juce::NotificationType::dontSendNotification);
        }
        if(selection == 3)
        {
            bpmLabel.setText("1/4",  juce::NotificationType::dontSendNotification);
        }
        if(selection == 4)
        {
            bpmLabel.setText("1/3",  juce::NotificationType::dontSendNotification);
        }
        if(selection == 5)
        {
            bpmLabel.setText("1/2",  juce::NotificationType::dontSendNotification);
        }
        if(selection == 6)
        {
            bpmLabel.setText("1/1",  juce::NotificationType::dontSendNotification);
        }
        
        
        if(presetBpmMode)
        {
            timeKnob.setVisible(false);
            timeLabel.setVisible(false);
            bpmSelectButton.setVisible(true);
            bpmKnob.setVisible(true);
        }
        else
        {
            timeKnob.setVisible(true);
            timeLabel.setVisible(true);
            bpmSelectButton.setVisible(false);
            bpmKnob.setVisible(false);
        }
        
        
    }
};
