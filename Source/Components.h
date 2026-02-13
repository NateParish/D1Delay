#pragma once
#include "Presets.h"
#include "CustomLF.h"

class ColourChangeButton final : public juce::TextButton,
public juce::ChangeListener
{
public:
    ColourChangeButton()
        : juce::TextButton ("Click to change colour...")
    {
        setSize (10, 24);
        changeWidthToFitText();
    }

    void clicked() override
    {
        auto colourSelector = std::make_unique<juce::ColourSelector> (juce::ColourSelector::showAlphaChannel
                                                                | juce::ColourSelector::showColourAtTop
                                                                | juce::ColourSelector::editableColour
                                                                | juce::ColourSelector::showSliders
                                                                | juce::ColourSelector::showColourspace);

        colourSelector->setName ("background");
        colourSelector->setCurrentColour (findColour (juce::TextButton::buttonColourId));
        colourSelector->addChangeListener (this);
        colourSelector->setColour (juce::ColourSelector::backgroundColourId, juce::Colours::transparentBlack);
        colourSelector->setSize (300, 400);

        juce::CallOutBox::launchAsynchronously (std::move (colourSelector), getScreenBounds(), nullptr);
    }

    using juce::TextButton::clicked;

    void changeListenerCallback (juce::ChangeBroadcaster* source) override
    {
        if (auto* cs = dynamic_cast<juce::ColourSelector*> (source))
            setColour (juce::TextButton::buttonColourId, cs->getCurrentColour());
    }
};


class DarkLayer : public juce::Component
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
        g.fillAll(juce::Colour::fromRGB(0,0,0).withAlpha(0.8f));
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

class ThemeSettingsComponent : public juce::Component,
                                public juce::Button::Listener
{
public:
    ThemeSettingsComponent()
    {
        leftColorButton.setButtonText("Set Left Color");
        rightColorButton.setButtonText("Set Right Color");
        eqColorButton.setButtonText("Set EQ Color");
        cancelButton.setButtonText("Cancel");
        cancelButton.addListener(this);
        saveButton.setButtonText("Save");

        addAndMakeVisible(leftColorButton);
        addAndMakeVisible(rightColorButton);
        addAndMakeVisible(eqColorButton);
        addAndMakeVisible(cancelButton);
        addAndMakeVisible(saveButton);
        
        ;
        
    }
    
    void buttonClicked(juce::Button* button) override
    {
        
        if (button == &cancelButton)
        {
            DBG("Cancel Clicked");
            
            cancelButtonCallback();
        }
    }
    void setCancelButtonCallback(std::function<void()> newCallback)
    {
        DBG("SETTING CANCEL CALLBACK - component");
        cancelButtonCallback = std::move(newCallback);
    }
    
    void resized() override
    {
        auto area = getLocalBounds().reduced(10);
        auto rowHeight = 40;

        leftColorButton.setBounds(area.removeFromTop(rowHeight).reduced(0, 5));
        rightColorButton.setBounds(area.removeFromTop(rowHeight).reduced(0, 5));
        eqColorButton.setBounds(area.removeFromTop(rowHeight).reduced(0, 5));
        cancelButton.setBounds(area.removeFromTop(rowHeight).reduced(0, 5));
        saveButton.setBounds(area.removeFromTop(rowHeight).reduced(0, 5));
    }

private:
    ColourChangeButton leftColorButton;
    ColourChangeButton rightColorButton;
    ColourChangeButton eqColorButton;
    
    juce::TextButton cancelButton;
    juce::TextButton saveButton;
    std::function<void()> cancelButtonCallback;
    
};


class MenuBarBackground : public juce::Component
{
    
private:
    
    std::shared_ptr<juce::Drawable> profileImage;

public:
    
    MenuBarBackground()
    {
        juce::MemoryInputStream svgStream(BinaryData::AccountIcon_svg,
                                          BinaryData::AccountIcon_svgSize,
                                          false);
        juce::String profileImageContent = svgStream.readEntireStreamAsString();
        juce::XmlDocument profileSVGDoc(profileImageContent);
        
        std::unique_ptr<juce::XmlElement> profileImageXml(profileSVGDoc.getDocumentElement());
           
        if (profileImageXml != nullptr)
        {
            profileImage = juce::Drawable::createFromSVG(*profileImageXml);
        }
        
        
    }
    
    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0, 0, 0));
        if (profileImage != nullptr)
            {
                juce::Rectangle<float> drawArea(getWidth()*.9, 0, getWidth()*.05, getHeight() * .90);
                profileImage->drawWithin(g, drawArea, juce::RectanglePlacement::centred, 1.0f);
            }
        
    }
    
    void resized() override
    {
        
    }
    
    private:
    
    
};

struct PresetMenuItem : public juce::PopupMenu::CustomComponent
{
    juce::String presetName;
    std::function<void()> onLoad;
    std::function<void()> onOptions;

    PresetMenuItem(const juce::String& name,
                   std::function<void()> onLoadFn,
                   std::function<void()> onOptionsFn)
        : juce::PopupMenu::CustomComponent(false),
          presetName(name),
          onLoad(onLoadFn),
          onOptions(onOptionsFn)
    {}

    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().reduced(4);
        g.setColour(juce::Colours::white);
        g.setFont(15.0f);
        g.drawText(presetName, bounds.removeFromLeft(getWidth() - 30), juce::Justification::centredLeft);

        // Draw gear icon
        auto iconArea = juce::Rectangle<int>(getWidth() - 24, 4, 16, 16);
        g.setColour(juce::Colours::lightgrey);
        g.fillEllipse(iconArea.toFloat());
        g.setColour(juce::Colours::black);
        g.drawEllipse(iconArea.toFloat(), 1.0f);
    }

    void mouseUp(const juce::MouseEvent& e) override
    {
        if (e.x > getWidth() - 30)
        {
            if (onOptions) onOptions(); // Clicked gear icon
        }
        else
        {
            if (onLoad) onLoad(); // Clicked text area
        }
    }

    void getIdealSize(int& width, int& height) override
    {
        width = 200;
        height = 24;
    }
};




class MenuBar final : public juce::Component
{
    
    public:
    
    MenuBar(D1DelayAudioProcessor& p)
    : audioProcessor (p)
    {
        
        preset1.setName("subtle vocal delay");
        preset1.setLevel(2.5);
        preset1.setFeedback(7.5);
        preset1.setTime(750);
        preset1.setTimeBpm(500);
        preset1.setWidth(6.6);
        preset1.setLeftLevel(8.0);
        preset1.setRightLevel(8.0);
        preset1.setPingPong(true);
        preset1.setLeftMuted(false);
        preset1.setRightMuted(false);
        preset1.setHighPass(420);
        preset1.setLowPass(840);
        
        vocalSlapback.setName("Vocal Slapback");
        vocalSlapback.setLevel(2.5f);
        vocalSlapback.setFeedback(1.7);
        vocalSlapback.setBpmKnobSelection(2);
        vocalSlapback.setTime(250);
        vocalSlapback.setTimeBpm(500);
        vocalSlapback.setWidth(10.f);
        vocalSlapback.setLeftLevel(10.f);
        vocalSlapback.setRightLevel(10.f);
        vocalSlapback.setPingPong(false);
        vocalSlapback.setBpmMode(true);
        vocalSlapback.setLeftMuted(false);
        vocalSlapback.setRightMuted(false);
        vocalSlapback.setHighPass(72);
        vocalSlapback.setLowPass(3400);
        
        slapback.setName("Fast Slapback Echo");
        slapback.setLevel(7.0f);
        slapback.setFeedback(1.4f);
        slapback.setTime(90);
        slapback.setWidth(5.0f);
        slapback.setLeftLevel(10.0f);
        slapback.setRightLevel(10.0f);
        slapback.setPingPong(false);
        slapback.setBpmMode(false);
        slapback.setLeftMuted(false);
        slapback.setRightMuted(false);
        slapback.setHighPass(120);
        slapback.setLowPass(18000);
        
        dottedDreams.setName("Dotted Dreams");
        dottedDreams.setLevel(5.5f);
        dottedDreams.setFeedback(5.8f);
        dottedDreams.setBpmKnobSelection(2); // 1/8
        dottedDreams.setWidth(8.0f);
        dottedDreams.setLeftLevel(10.0f);
        dottedDreams.setRightLevel(10.0f);
        dottedDreams.setPingPong(true);
        dottedDreams.setBpmMode(true);
        dottedDreams.setLeftMuted(false);
        dottedDreams.setRightMuted(false);
        dottedDreams.setHighPass(100);
        dottedDreams.setLowPass(16000);
        
        loFiTape.setName("Lo-Fi Tape");
        loFiTape.setLevel(6.0f);
        loFiTape.setFeedback(5.1f);
        loFiTape.setBpmKnobSelection(3); // 1/4
        loFiTape.setWidth(4.0f);
        loFiTape.setLeftLevel(10.0f);
        loFiTape.setRightLevel(10.0f);
        loFiTape.setPingPong(false);
        loFiTape.setBpmMode(true);
        loFiTape.setLeftMuted(false);
        loFiTape.setRightMuted(false);
        loFiTape.setHighPass(200);
        loFiTape.setLowPass(8000);
        
        dub.setName("Dub");
        dub.setLevel(6.0f);
        dub.setFeedback(8.0f);
        dub.setBpmKnobSelection(3); // 1/4
        dub.setWidth(10.0f);
        dub.setLeftLevel(10.0f);
        dub.setRightLevel(10.0f);
        dub.setPingPong(false);
        dub.setBpmMode(true);
        dub.setLeftMuted(false);
        dub.setRightMuted(false);
        dub.setHighPass(120);
        dub.setLowPass(13000);
        
        ambientCloud.setName("Ambient Cloud");
        ambientCloud.setLevel(4.5f);
        ambientCloud.setFeedback(7.2f);
        ambientCloud.setBpmKnobSelection(5); // 1/1
        ambientCloud.setWidth(10.0f);
        ambientCloud.setLeftLevel(10.0f);
        ambientCloud.setRightLevel(10.0f);
        ambientCloud.setPingPong(true);
        ambientCloud.setBpmMode(true);
        ambientCloud.setLeftMuted(false);
        ambientCloud.setRightMuted(false);
        ambientCloud.setHighPass(80);
        ambientCloud.setLowPass(16000);
        
        
        smallRoom.setName("Small Room");
        smallRoom.setLevel(5.0f);
        smallRoom.setFeedback(5.0f);
        smallRoom.setTime(11);
        smallRoom.setWidth(10.f);
        smallRoom.setLeftLevel(10.f);
        smallRoom.setRightLevel(10.f);
        smallRoom.setPingPong(false);
        smallRoom.setBpmMode(false);
        smallRoom.setLeftMuted(false);
        smallRoom.setRightMuted(false);
        smallRoom.setHighPass(150);
        smallRoom.setLowPass(12800);
        
        mediumRoom.setName("Medium Room");
        mediumRoom.setLevel(5.0f);
        mediumRoom.setFeedback(5.0f);
        mediumRoom.setBpmKnobSelection(3);
        mediumRoom.setTime(25);
        mediumRoom.setWidth(10.f);
        mediumRoom.setLeftLevel(10.f);
        mediumRoom.setRightLevel(10.f);
        mediumRoom.setPingPong(false);
        mediumRoom.setBpmMode(false);
        mediumRoom.setLeftMuted(false);
        mediumRoom.setRightMuted(false);
        mediumRoom.setHighPass(160);
        mediumRoom.setLowPass(9500);
        
        largeRoom.setName("Large Room");
        largeRoom.setLevel(5.0f);
        largeRoom.setFeedback(5.0f);
        largeRoom.setBpmKnobSelection(3);
        largeRoom.setTime(55);
        largeRoom.setWidth(10.f);
        largeRoom.setLeftLevel(10.f);
        largeRoom.setRightLevel(10.f);
        largeRoom.setPingPong(false);
        largeRoom.setBpmMode(false);
        largeRoom.setLeftMuted(false);
        largeRoom.setRightMuted(false);
        largeRoom.setHighPass(160);
        largeRoom.setLowPass(7000);
        
        wideStereo.setName("Wide Stereo");
        wideStereo.setLevel(6.0f);
        wideStereo.setFeedback(5.0f);
        wideStereo.setBpmKnobSelection(3);
        wideStereo.setWidth(10.f);
        wideStereo.setLeftLevel(10.f);
        wideStereo.setRightLevel(10.f);
        wideStereo.setPingPong(true);
        wideStereo.setBpmMode(true);
        wideStereo.setLeftMuted(false);
        wideStereo.setRightMuted(false);
        wideStereo.setHighPass(50);
        wideStereo.setLowPass(20000);
        
        presetsButton.setLookAndFeel(&buttonLF);
        prefsButton.setLookAndFeel(&buttonLF);
        addAndMakeVisible(background);
        addAndMakeVisible(presetsButton);
        presetsButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        addAndMakeVisible(profileButton);
        
        // Presets Menu
        presetsButton.setButtonText("Presets");
        presetsButton.onClick = [&]
        {
            juce::PopupMenu menu;
            
            
            menu.addSectionHeader("Presets");
            
            menu.addItem(ambientCloud.getName(), [this] {
                loadPresetCallback(ambientCloud);
            });
            menu.addItem(dottedDreams.getName(), [this] {
                loadPresetCallback(dottedDreams);
            });
            menu.addItem(dub.getName(), [this] {
                loadPresetCallback(dub);
            });
            menu.addItem(slapback.getName(), [this] {
                loadPresetCallback(slapback);
            });
            menu.addItem(loFiTape.getName(), [this] {
                loadPresetCallback(loFiTape);
            });
            menu.addItem(vocalSlapback.getName(), [this] {
                loadPresetCallback(vocalSlapback);
            });
            menu.addItem(wideStereo.getName(), [this] {
                loadPresetCallback(wideStereo);
            });

            menu.addSectionHeader("User Presets");

            juce::File presetDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                                    .getChildFile("ParishAudio")
                                    .getChildFile(appName)
                                    .getChildFile("UserPresets");

            if (presetDir.exists() && presetDir.isDirectory())
            {
                juce::Array<juce::File> fileArray = presetDir.findChildFiles(juce::File::findFiles, false, "*.preset");

                // Copy to std::vector for sorting
                std::vector<juce::File> files(fileArray.begin(), fileArray.end());

                // Sort alphabetically by filename (case-insensitive)
                std::sort(files.begin(), files.end(), [](const juce::File& a, const juce::File& b) {
                    return a.getFileNameWithoutExtension().compareIgnoreCase(b.getFileNameWithoutExtension()) < 0;
                });

                for (const auto& file : files)
                {
                    const auto presetName = file.getFileNameWithoutExtension();

                    juce::PopupMenu subMenu;
                    // Load (wrapped in callAsync for thread safety)
                    subMenu.addItem("Load", [this, file, presetName]
                    {
                        juce::MessageManager::callAsync([=]
                        {
                            juce::PropertiesFile::Options opts;
                            opts.applicationName = presetName;
                            opts.storageFormat = juce::PropertiesFile::StorageFormat::storeAsXML;

                            juce::PropertiesFile propFile(file, opts);

                            DelayPreset loaded(
                                presetName.toStdString(),
                                propFile.getDoubleValue("Level", 5.0f),
                                propFile.getDoubleValue("HighPass", 5.0f),
                                propFile.getDoubleValue("LowPass", 5.0f),
                                propFile.getDoubleValue("Feedback", 5.0f),
                                propFile.getDoubleValue("Time", 500.0f),
                                propFile.getDoubleValue("TimeBPM", 5.f),
                                propFile.getDoubleValue("LeftLevel", 10.0f),
                                propFile.getDoubleValue("RightLevel", 10.0f),
                                propFile.getDoubleValue("Width", 10.0f),
                                propFile.getBoolValue("LeftMuted", false),
                                propFile.getBoolValue("RightMuted", false),
                                propFile.getBoolValue("PingPong", false),
                                propFile.getBoolValue("InBpmMode", true),
                                propFile.getDoubleValue("BpmKnobSelection", 4.0f)
                            );

                            if (loadPresetCallback)
                                loadPresetCallback(loaded);
                        });
                    });

                    // Rename
                    subMenu.addItem("Rename", [this, file, presetDir]
                    {
                        auto* alert = new juce::AlertWindow("Rename Preset",
                                                            "Enter a new name:",
                                                            juce::AlertWindow::AlertIconType::QuestionIcon);
                        alert->setBounds(getWidth()*.5 - getWidth() * .15, getHeight()*.3, getWidth() * .3, getHeight() * .3);
                        alert->setColour( juce::AlertWindow::backgroundColourId, juce::Colour::fromRGB(20,21,22));
                        alert->addTextEditor("newName", file.getFileNameWithoutExtension(), "New name:");
                        alert->getTextEditor("newName")->setColour(juce::TextEditor::backgroundColourId, juce::Colour::fromRGBA(20,21,22,0));
                        alert->addButton("Rename", 1);
                        alert->addButton("Cancel", 0);
                        alert->getButton(0)->setMouseCursor(juce::MouseCursor::PointingHandCursor);
                        alert->getButton(1)->setMouseCursor(juce::MouseCursor::PointingHandCursor);
                        alert->getButton(0)->setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
                        alert->getButton(1)->setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
                        alert->getButton(0)->setLookAndFeel(&uniformButtonLF);
                        alert->getButton(1)->setLookAndFeel(&uniformButtonLF);
                        alert->getButton(0)->setSize(80,30);
                        alert->getButton(1)->setSize(80,30);

                        alert->enterModalState(true, juce::ModalCallbackFunction::create([this, file, presetDir, alert](int result)
                        {
                            std::unique_ptr<juce::AlertWindow> cleaner(alert);
                            if (result == 1)
                            {
                                auto newName = alert->getTextEditor("newName")->getText().trim();
                                if (newName.isNotEmpty())
                                {
                                    juce::File newFile = presetDir.getChildFile(newName + ".preset");
                                    auto currentName = file.getFileNameWithoutExtension();

                                    if (newName == currentName)
                                    {
                                        // Name hasn't changed — do nothing
                                        return;
                                    }
                                    else if (!newFile.exists())
                                    {
                                        // Name changed and does not conflict — perform rename
                                        if (presetLock.enter(5000))
                                        {
                                            const juce::ScopedLock lock(presetFileLock);
                                            file.moveFileTo(newFile);

                                            auto* successAlert = new juce::AlertWindow("Preset Renamed",
                                                                                       "The preset was successfully renamed to \"" + newFile.getFileNameWithoutExtension() + "\".",
                                                                                       juce::AlertWindow::InfoIcon);

                                            successAlert->setColour( juce::AlertWindow::backgroundColourId, juce::Colour::fromRGB(20,21,22));
                                            successAlert->addButton("OK", 1);
                                            successAlert->getButton(0)->setMouseCursor(juce::MouseCursor::PointingHandCursor);
                                            successAlert->getButton(0)->setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
                                            successAlert->setLookAndFeel(&uniformButtonLF);
                                            successAlert->getButton(0)->setSize(60, 30);

                                            successAlert->enterModalState(true, juce::ModalCallbackFunction::create([successAlert](int) {
                                                std::unique_ptr<juce::AlertWindow> cleaner(successAlert);
                                            }));

                                            presetLock.exit();
                                        }
                                    }
                                    else
                                    {
                                        // Name changed but conflicts with another preset — show alert
                                        auto* conflictAlert = new juce::AlertWindow("Error",
                                                                                    "A preset with that name already exists.",
                                                                                    juce::AlertWindow::WarningIcon);
                                        conflictAlert->setColour( juce::AlertWindow::backgroundColourId, juce::Colour::fromRGB(20,21,22));
                                        conflictAlert->addButton("OK", 1);
                                        conflictAlert->getButton(0)->setMouseCursor(juce::MouseCursor::PointingHandCursor);
                                        conflictAlert->getButton(0)->setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
                                        conflictAlert->getButton(0)->setLookAndFeel(&uniformButtonLF);
                                        conflictAlert->getButton(0)->setSize(80, 30);

                                        conflictAlert->enterModalState(true, juce::ModalCallbackFunction::create(
                                            [conflictAlert](int) {
                                                std::unique_ptr<juce::AlertWindow> cleaner(conflictAlert);
                                            }));
                                    }
                                }
                            }
                        }));
                    });

                    // Delete preset
                    subMenu.addItem("Delete", [this, file]
                    {
                        auto* confirmAlert = new juce::AlertWindow("Are you sure?",
                                                                   "Are you sure you want to delete the preset \"" + file.getFileNameWithoutExtension() + "\"?",
                                                                   juce::AlertWindow::WarningIcon);
                        
                        confirmAlert->setBounds(getWidth()*.5 - getWidth() * .15, getHeight()*.3, getWidth() * .3, getHeight() * .3);
                        confirmAlert->addButton("Delete", 1);
                        confirmAlert->addButton("Cancel", 0);
                        confirmAlert->setColour( juce::AlertWindow::backgroundColourId, juce::Colour::fromRGB(20,21,22));
                        confirmAlert->getButton(0)->setMouseCursor(juce::MouseCursor::PointingHandCursor);
                        confirmAlert->getButton(1)->setMouseCursor(juce::MouseCursor::PointingHandCursor);
                        confirmAlert->getButton(0)->setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
                        confirmAlert->getButton(1)->setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
                        confirmAlert->getButton(0)->setSize(80,30);
                        confirmAlert->getButton(1)->setSize(80,30);
                        confirmAlert->getButton(0)->setLookAndFeel(&uniformButtonLF);
                        confirmAlert->getButton(1)->setLookAndFeel(&uniformButtonLF);

                        confirmAlert->enterModalState(true, juce::ModalCallbackFunction::create([this, file, confirmAlert](int result)
                        {
                            std::unique_ptr<juce::AlertWindow> cleaner(confirmAlert);
                            if (result == 1)
                            {
                                if (presetLock.enter(5000))  // wait up to 5 seconds
                                {
                                    const juce::ScopedLock lock(presetFileLock);
                                    file.deleteFile();
                                    
                                    
                                    auto* successAlert = new juce::AlertWindow("Preset Deleted",
                                                                               "Preset \"" + file.getFileNameWithoutExtension() + "\" was successfully deleted.",
                                                                               juce::AlertWindow::InfoIcon);
                                    successAlert->setBounds(getWidth()*.5 - getWidth() * .15, getHeight()*.3, getWidth() * .3, getHeight() * .3);
                                    successAlert->setColour( juce::AlertWindow::backgroundColourId, juce::Colour::fromRGB(20,21,22));
                                    successAlert->addButton("OK",1);
                                    successAlert->getButton(0)->setMouseCursor(juce::MouseCursor::PointingHandCursor);
                                    successAlert->getButton(0)->setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
                                    successAlert->getButton(0)->setLookAndFeel(&uniformButtonLF);
                                    successAlert->getButton(0)->setSize(60, 30);

                                    successAlert->enterModalState(true, juce::ModalCallbackFunction::create([successAlert](int) {
                                        std::unique_ptr<juce::AlertWindow> cleaner(successAlert);
                                    }));
                                                    
                                    presetLock.exit();
                                }
                                

                                
                            }
                        }));
                    });

                    menu.addSubMenu(presetName, subMenu);
                }
            }

            menu.addSeparator();
            menu.addItem("Save Current as Preset", [this] { savePreset(); });
            menu.setLookAndFeel(&menuLF); // Apply custom look
            menu.showMenuAsync(juce::PopupMenu::Options{}.withTargetComponent(presetsButton));
            
        };
        
        
        prefsButton.setButtonText("Preferences");
        prefsButton.onClick = [&]
        {
            juce::PopupMenu menu;
            ColourChangeButton colorButton;
            addAndMakeVisible(&colorButton);
            menu.addItem("Audio Settings", [] { juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon, "Preferences", "Open audio settings..."); });

            menu.addItem("Colors and Themes", [this] { handleOpenPreferences(); });

                menu.showMenuAsync(juce::PopupMenu::Options{}.withTargetComponent(prefsButton));
    

            menu.showMenuAsync(juce::PopupMenu::Options{}.withTargetComponent(prefsButton));
        };

        // Profile Icon
        juce::Image img = juce::ImageCache::getFromMemory(BinaryData::MenuIcon_svg, BinaryData::MenuIcon_svgSize);
        
        profileButton.setTooltip("Profile");
        profileButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        profileButton.onClick = [&]
        {
            juce::PopupMenu menu;
            menu.setLookAndFeel(&menuLF);
            menu.addItem("Logout", [this] { handleLogout(); });

            menu.showMenuAsync(juce::PopupMenu::Options{}.withTargetComponent(profileButton));
        };
    }

    
    void resized() override
    {
        background.setBounds(0,0, getWidth(), getHeight());
        presetsButton.setBounds(getWidth()*.05, 0, getWidth()*.07, getHeight());
        prefsButton.setBounds(getWidth()*.135, 0, getWidth()*.10, getHeight());
        profileButton.setBounds(getWidth()*.9, 0, getWidth()*.05, getHeight());
    }
    
    void setAppName(juce::String appName)
    {
        this->appName = appName;
    }
    
    void setLogoutButtonCallback(std::function<void()> newCallback)
    {
        logoutButtonCallback = std::move(newCallback);
    }
    
    
    void setLoadPresetCallback(std::function<void(DelayPreset)> newCallback)
    {
        loadPresetCallback = std::move(newCallback);
    }
    void setOpenPreferencesCallback(std::function<void()> newCallback)
    {
        openPreferencesCallback = std::move(newCallback);
    }
    

private:
    
    MenuButtonLF buttonLF;
    BlueButton1 buttonLF2;
    BlueButton1 buttonLF3;
    BlueButton1 buttonLF4;
    BlueButton1 buttonLF5;
    UniformButtonLookAndFeel uniformButtonLF;
    
    MenuLookAndFeel menuLF;
    MenuBarBackground background;
    juce::TextButton presetsButton, prefsButton;
    juce::ImageButton profileButton;
    juce::String appName;
    DelayPreset preset1;
    DelayPreset preset2 = DelayPreset("wide stereo", 0.6, 1.0f, 0.0f, .75, 500, 500, 1.0, 1.0, 1.0, false, false, true, true, 3);
    
    DelayPreset vocalSlapback;
    DelayPreset smallRoom;
    DelayPreset mediumRoom;
    DelayPreset largeRoom;
    DelayPreset wideStereo = DelayPreset("wide stereo", 0.6, 1.0f, 0.0f, .75, 500, 500, 1.0, 1.0, 1.0, false, false, true, true, 3);
    
    DelayPreset slapback;
    DelayPreset dottedDreams;
    DelayPreset loFiTape;
    DelayPreset dub;
    DelayPreset ambientCloud;
    
    
    std::function<void()> logoutButtonCallback;
    std::function<void()> openPreferencesCallback;
    std::function<void(DelayPreset)> loadPresetCallback;
    juce::PropertiesFile::Options options;
    std::unique_ptr<juce::PropertiesFile> propertiesFile;
    juce::InterProcessLock interProcessLock { "ParishAudioSavePresetLock" };
    juce::String lastPresetNameAttempted;
    juce::CriticalSection presetFileLock;
    juce::InterProcessLock presetLock { "PresetFileAccessLock" };
    juce::CriticalSection presetMutex;
    
    
    D1DelayAudioProcessor& audioProcessor;

    void savePreset()
    {
        
        auto defaultName = lastPresetNameAttempted.isNotEmpty()
            ? lastPresetNameAttempted
            : getAvailablePresetName("My Preset");

        auto* alert = new juce::AlertWindow("Save Preset",
                                            "Enter a name for your preset:",
                                            juce::AlertWindow::QuestionIcon);
        alert->setSize(getWidth() * .3, getHeight() * .3);
        alert->setBounds(getWidth()*.5 - getWidth() * .15, getHeight()*.3, getWidth() * .3, getHeight() * .3);
        alert->addTextEditor("presetName", defaultName, "Preset Name:");
        alert->addButton("Save", 1, juce::KeyPress(juce::KeyPress::returnKey));
        alert->addButton("Cancel", 0, juce::KeyPress(juce::KeyPress::escapeKey));
        alert->setColour( juce::AlertWindow::backgroundColourId, juce::Colour::fromRGB(20,21,22));
        alert->getTextEditor("presetName")->setColour(juce::TextEditor::backgroundColourId, juce::Colour::fromRGBA(20,21,22, 0));
        alert->getButton(0)->setMouseCursor(juce::MouseCursor::PointingHandCursor);
        alert->getButton(1)->setMouseCursor(juce::MouseCursor::PointingHandCursor);
        alert->getButton(0)->setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
        alert->getButton(1)->setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
        alert->getButton(0)->setLookAndFeel(&uniformButtonLF);
        alert->getButton(1)->setLookAndFeel(&uniformButtonLF);
        alert->getButton(0)->setSize(80,30);
        alert->getButton(1)->setSize(80,30);

        alert->enterModalState(true, juce::ModalCallbackFunction::create(
            [this, alert](int result)
            {
                std::unique_ptr<juce::AlertWindow> cleaner(alert);

                if (result == 1)
                {
                    auto presetName = alert->getTextEditor("presetName")->getText().trim();
                    if (presetName.isNotEmpty())
                    {
                        lastPresetNameAttempted = presetName;

                        juce::File defaultFile = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                            .getChildFile("ParishAudio")
                            .getChildFile(appName)
                            .getChildFile("UserPresets");

                        defaultFile.createDirectory();

                        juce::File outputFile = defaultFile.getChildFile(presetName + ".preset");

                        if (outputFile.existsAsFile())
                        {
                            auto* overwriteAlert = new juce::AlertWindow("Overwrite Preset?",
                                "A preset named \"" + presetName + "\" already exists. Overwrite it?",
                                juce::AlertWindow::WarningIcon);

                            overwriteAlert->addButton("Overwrite", 1);
                            overwriteAlert->addButton("Back", 2);
                            overwriteAlert->setColour( juce::AlertWindow::backgroundColourId, juce::Colour::fromRGB(20,21,22));
                            overwriteAlert->getButton(0)->setMouseCursor(juce::MouseCursor::PointingHandCursor);
                            overwriteAlert->getButton(1)->setMouseCursor(juce::MouseCursor::PointingHandCursor);
                            overwriteAlert->getButton(0)->setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
                            overwriteAlert->getButton(1)->setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
                            
                            overwriteAlert->getButton(0)->setSize(100,30);
                            overwriteAlert->getButton(1)->setSize(100,30);
                            
                            
                            overwriteAlert->getButton(0)->setLookAndFeel(&uniformButtonLF);
                            overwriteAlert->getButton(1)->setLookAndFeel(&uniformButtonLF);

                            overwriteAlert->enterModalState(true, juce::ModalCallbackFunction::create(
                                [this, overwriteAlert, outputFile, presetName](int overwriteResult)
                                {
                                    std::unique_ptr<juce::AlertWindow> cleaner(overwriteAlert);
                                    if (overwriteResult == 1)
                                    {
                                        writePresetToFile(outputFile, presetName);
                                    }
                                    else if (overwriteResult == 2)
                                    {
                                        savePreset(); // Go back to name input
                                    }
                                }));
                        }
                        else
                        {
                            writePresetToFile(outputFile, presetName);
                        }
                    }
                }
                else if (result == 0)
                {
                    lastPresetNameAttempted.clear(); // Clear on Cancel
                }
            }));
    }

    
    
    juce::String getAvailablePresetName(const juce::String& baseName)
    {
        juce::File presetDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
            .getChildFile("ParishAudio")
            .getChildFile(appName)
            .getChildFile("UserPresets");

        presetDir.createDirectory();

        juce::String name = baseName;
        int counter = 1;

        while (presetDir.getChildFile(name + ".preset").existsAsFile())
        {
            name = baseName + juce::String(counter++);
        }

        return name;
    }
    
    
    void writePresetToFile(const juce::File& outputFile, const juce::String& presetName)
    {
        
        if (presetLock.enter(5000))  // wait up to 5 seconds
            {
                juce::ScopedLock scopedLock(presetMutex); // in-process thread safety

                // Write preset data safely
                juce::PropertiesFile::Options options;
                options.applicationName = presetName;
                options.filenameSuffix = "";
                options.storageFormat = juce::PropertiesFile::StorageFormat::storeAsXML;
                options.commonToAllUsers = false;

                propertiesFile.reset(new juce::PropertiesFile(outputFile, options));

                propertiesFile->setValue("Level", audioProcessor.getLevel() * 10);
                propertiesFile->setValue("HighPass", audioProcessor.getHighPass());
                propertiesFile->setValue("LowPass", audioProcessor.getLowPass());
                propertiesFile->setValue("Feedback", audioProcessor.getFeedback() * 10);
                propertiesFile->setValue("Time", audioProcessor.getLeftLevel() * 1000);
                propertiesFile->setValue("TimeBPM", audioProcessor.getTimeBpm() * 10);
                propertiesFile->setValue("LeftLevel", audioProcessor.getLeftLevel() * 10);
                propertiesFile->setValue("RightLevel", audioProcessor.getRightLevel() * 10);
                propertiesFile->setValue("Width", audioProcessor.getWidth() * 10);
                propertiesFile->setValue("LeftMuted", audioProcessor.getLeftMuted());
                propertiesFile->setValue("RightMuted", audioProcessor.getRightMuted());
                propertiesFile->setValue("PingPong", audioProcessor.getWidth());
                propertiesFile->setValue("InBpmMode", audioProcessor.getBpmMode());
                propertiesFile->setValue("BpmKnobSelection", audioProcessor.getBPMSelection());

                auto* alert = new juce::AlertWindow("Preset Saved",
                    "The preset \"" + presetName + "\" was saved successfully.",
                    juce::AlertWindow::InfoIcon);
                alert->setBounds(getWidth()*.5 - getWidth() * .15, getHeight()*.3, getWidth() * .3, getHeight() * .3);
                alert->setColour( juce::AlertWindow::backgroundColourId, juce::Colour::fromRGB(20,21,22));
                alert->addButton("OK", 1);
                alert->getButton(0)->setMouseCursor(juce::MouseCursor::PointingHandCursor);
                alert->getButton(0)->setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
                alert->getButton(0)->setLookAndFeel(&uniformButtonLF);
                alert->getButton(0)->setSize(60, 30);

                alert->enterModalState(true, juce::ModalCallbackFunction::create([alert](int) {
                    std::unique_ptr<juce::AlertWindow> cleaner(alert);
                }));
                lastPresetNameAttempted.clear();
                presetLock.exit();
                return;
            }
            else
            {
                DBG("Failed to acquire lock for saving preset.");
                return;
            }
    
    }
    
    void handleLogout()
    {

        logoutButtonCallback();
    }
    
    void handleOpenPreferences()
    {

        if(openPreferencesCallback)
        {
            openPreferencesCallback();
        }
        else
        {
            DBG("openPreferencesCallback NOT SET");
        }
    }
    
    
};




