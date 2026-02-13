#pragma once
#include "ThemeColors.h"

class ButtonStyle1LF : public juce::LookAndFeel_V4 {
public:
    
    
    void drawButtonText (juce::Graphics& g, juce::TextButton& button,
        bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/)
    {
        juce::Font font = juce::Font(juce::FontOptions(button.getHeight () * 0.6f));
        g.setFont (font);
        g.setColour (button.findColour (button.getToggleState () ?
        juce::TextButton::textColourOnId    : juce::TextButton::textColourOffId)
            .withMultipliedAlpha (button.isEnabled () ? 1.0f : 0.5f));

        const int yIndent = button.proportionOfHeight (0.1f);
        const int cornerSize = juce::jmin (button.getHeight (), button.getWidth ()) / 2;
        const int leftIndent = cornerSize / (button.isConnectedOnLeft () ?
                  yIndent * 2 : yIndent);
        const int rightIndent = cornerSize / (button.isConnectedOnRight () ?
                  yIndent * 2 : yIndent);
        const int textWidth = button.getWidth () - leftIndent - rightIndent;

        if (textWidth > 0)
            g.drawFittedText (button.getButtonText (),
                leftIndent, yIndent, textWidth, button.getHeight () - yIndent * 2,
                juce::Justification::centred, 2, 0.5f);
    }
};

class BlueButton1 : public juce::LookAndFeel_V4 {
    
    private:
    
    ThemeColors colors;
    
public:
    
    void drawButtonText (juce::Graphics& g, juce::TextButton& button,
        bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/) override
    {

        juce::Font font = juce::Font(juce::FontOptions(button.getHeight () * 0.6f));
        g.setFont (font);
        g.setColour (button.findColour (button.getToggleState () ?
        juce::TextButton::textColourOnId    : juce::TextButton::textColourOffId)
            .withMultipliedAlpha (button.isEnabled () ? 1.0f : 0.5f));

        const int yIndent = button.proportionOfHeight (0.1f);
        const int cornerSize = juce::jmin (button.getHeight (), button.getWidth ()) / 2;

        const int leftIndent = cornerSize / (button.isConnectedOnLeft () ?
                  yIndent * 2 : yIndent);
        const int rightIndent = cornerSize / (button.isConnectedOnRight () ?
                  yIndent * 2 : yIndent);
        const int textWidth = button.getWidth () - leftIndent - rightIndent;
        if (textWidth > 0)
            g.drawFittedText (button.getButtonText (),
                leftIndent, yIndent, textWidth, button.getHeight () - yIndent * 2,
                juce::Justification::centred, 2, 0.5f);
    }
};



class LogoutButtonLF : public juce::LookAndFeel_V4 {
public:
    
    void drawButtonText (juce::Graphics& g, juce::TextButton& button,
        bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/)
    {

        juce::Font font = juce::Font(juce::FontOptions(button.getHeight () * 0.6f));
        g.setFont (font);
        g.setColour (button.findColour (button.getToggleState () ?
        juce::TextButton::textColourOnId    : juce::TextButton::textColourOffId)
            .withMultipliedAlpha (button.isEnabled () ? 1.0f : 0.5f));

        const int yIndent = button.proportionOfHeight (0.1f);
        const int cornerSize = juce::jmin (button.getHeight (), button.getWidth ()) / 2;
        const int leftIndent = cornerSize / (button.isConnectedOnLeft () ?
                  yIndent * 2 : yIndent);
        const int rightIndent = cornerSize / (button.isConnectedOnRight () ?
                  yIndent * 2 : yIndent);
        const int textWidth = button.getWidth () - leftIndent - rightIndent;

        if (textWidth > 0)
            g.drawFittedText (button.getButtonText (),
                leftIndent, yIndent, textWidth, button.getHeight () - yIndent * 2,
                juce::Justification::centred, 2, 0.5f);
    }
};



class LoginButtonLF : public juce::LookAndFeel_V4 {
public:
    
    
    void drawButtonText (juce::Graphics& g, juce::TextButton& button,
        bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/)
    {
        juce::Font font = juce::Font(juce::FontOptions(button.getHeight () * 0.6f));
        g.setFont (font);
        g.setColour (button.findColour (button.getToggleState () ?
        juce::TextButton::textColourOnId    : juce::TextButton::textColourOffId)
            .withMultipliedAlpha (button.isEnabled () ? 1.0f : 0.5f));

        const int yIndent = button.proportionOfHeight (0.1f);
        const int cornerSize = juce::jmin (button.getHeight (), button.getWidth ()) / 2;
        const int leftIndent = cornerSize / (button.isConnectedOnLeft () ?
                  yIndent * 2 : yIndent);
        const int rightIndent = cornerSize / (button.isConnectedOnRight () ?
                  yIndent * 2 : yIndent);
        const int textWidth = button.getWidth () - leftIndent - rightIndent;

        if (textWidth > 0)
            g.drawFittedText (button.getButtonText (),
                leftIndent, yIndent, textWidth, button.getHeight () * 0.8 - yIndent * 2,
                juce::Justification::centred, 2, 0.5f);
    }
};


class MenuLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override
    {
        g.setColour(juce::Colour::fromRGB(40,50,55));
        g.fillAll(); // dark blue-gray background
    }

    void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
                           bool isSeparator, bool isActive, bool isHighlighted, bool isTicked,
                           bool hasSubMenu, const juce::String& text, const juce::String& shortcutKeyText,
                           const juce::Drawable* icon, const juce::Colour* textColour) override
    {
        if (isSeparator)
        {
            g.setColour(juce::Colours::grey);
            g.drawLine((float)area.getX(), (float)(area.getCentreY()),
                       (float)area.getRight(), (float)(area.getCentreY()));
        }
        else
        {
            auto textArea = area.reduced(10);

            if (isHighlighted)
            {
                g.setColour(juce::Colour(82, 109, 130)); // hover color
                g.fillRect(area);
            }

            g.setColour(textColour != nullptr ? *textColour
                                              : isActive ? juce::Colours::white
                                                         : juce::Colours::grey);
            g.setFont(15.0f);
            g.drawText(text, textArea, juce::Justification::centredLeft, true);
        }
    }
};

class UniformButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                              const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override
    {
        auto baseColour = backgroundColour;

        // Disable the "default button" visual highlight effect
        const bool isDefault = button.hasKeyboardFocus (true) && button.getWantsKeyboardFocus();
        if (isDefault)
            baseColour = backgroundColour; // ignore default focus highlight

        if (shouldDrawButtonAsDown)
            baseColour = baseColour.darker(0.2f);
        else if (shouldDrawButtonAsHighlighted)
            baseColour = baseColour.brighter(0.1f);

        g.setColour(baseColour);
        g.fillRoundedRectangle(button.getLocalBounds().toFloat(), 4.0f);
    }
};

class MenuButtonLF : public juce::LookAndFeel_V4
{
public:
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                              const juce::Colour& backgroundColour,
                              bool isMouseOverButton, bool isButtonDown) override
    {
        // Transparent background
        juce::ignoreUnused(backgroundColour, isMouseOverButton, isButtonDown);
        g.fillAll(juce::Colours::transparentBlack);
        
    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& button,
                        bool /*isMouseOverButton*/, bool /*isButtonDown*/) override
    {
        g.setColour(juce::Colour(250, 250, 252));
        g.setFont(button.getHeight() * 0.5f);
        g.drawFittedText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, 1);
    }
};


class TextBoxLF : public juce::LookAndFeel_V4
{
    
    public:
    void fillTextEditorBackground(juce::Graphics &g, int width, int height, juce::TextEditor &textEditor)
    {
        g.setColour(juce::Colour::fromRGBA(255,255,0,0));
        g.fillAll();
    }
    
    void drawTextEditorOutline(juce::Graphics &g, int width, int height, juce::TextEditor &textEditor)
    {
            // don't draw anything
    }
    
};

class HoverImageButtonLF : public juce::LookAndFeel_V4
{
public:
    HoverImageButtonLF()
    {
        // Load normal icon from BinaryData
        {
            juce::MemoryInputStream svgStream(BinaryData::MenuIcon_svg, BinaryData::MenuIcon_svgSize, false);
            juce::String svgContent = svgStream.readEntireStreamAsString();
            juce::XmlDocument svgDoc(svgContent);
            std::unique_ptr<juce::XmlElement> svgXml(svgDoc.getDocumentElement());

            if (svgXml)
                normalDrawable = juce::Drawable::createFromSVG(*svgXml);
        }

        // Load hover icon from BinaryData
        {
            juce::MemoryInputStream svgStream(BinaryData::MenuIconBright_svg, BinaryData::MenuIconBright_svgSize, false);
            juce::String svgContent = svgStream.readEntireStreamAsString();
            juce::XmlDocument svgDoc(svgContent);
            std::unique_ptr<juce::XmlElement> svgXml(svgDoc.getDocumentElement());

            if (svgXml)
                hoverDrawable = juce::Drawable::createFromSVG(*svgXml);
        }
    }

    void drawButtonBackground(juce::Graphics&, juce::Button&, const juce::Colour&, bool, bool) override
    {
        // No background
    }

    void drawButtonImage(juce::Graphics& g, juce::ImageButton& button, bool isMouseOver, bool isButtonDown)
    {
        auto bounds = button.getLocalBounds().toFloat();
        auto* drawableToUse = (isMouseOver || isButtonDown) ? hoverDrawable.get() : normalDrawable.get();
        normalDrawable->drawWithin(g, bounds, juce::RectanglePlacement::centred, 1.0f);
        if (drawableToUse)
        {
            drawableToUse->drawWithin(g, bounds, juce::RectanglePlacement::centred, 1.0f);
        }
    }

private:
    std::unique_ptr<juce::Drawable> normalDrawable;
    std::unique_ptr<juce::Drawable> hoverDrawable;
};

