#pragma once


class MenuButtonIcon : public juce::Component
{
    
private:
    
    
    std::unique_ptr<juce::Drawable> menuIcon;
    
    bool on = true;

public:
    
    MenuButtonIcon()
    {
        juce::MemoryInputStream svgStream(BinaryData::MenuIcon_svg, BinaryData::MenuIcon_svgSize, false);

        juce::String svgContent = svgStream.readEntireStreamAsString();
        juce::XmlDocument svgDoc(svgContent);
        std::unique_ptr<juce::XmlElement> svgXml(svgDoc.getDocumentElement());
        

        if (svgXml != nullptr)
        {
            menuIcon = juce::Drawable::createFromSVG(*svgXml);
            
        }
        
        
    }
    
    void paint(juce::Graphics& g) override
    {
        juce::Rectangle<float> drawArea(0, 0,getWidth(),getHeight());
        menuIcon->drawWithin(g, drawArea, juce::RectanglePlacement::centred, 1.0f);
    }

    
    void resized() override
    {
        
    }
    
    void switchToVisOn()
    {
        on = true;
    }
    void switchToVisOff()
    {
        on = false;
    }
    
};


class AppMenuBar : public juce::MenuBarModel
{
public:
    std::function<void(juce::String menu, juce::String item)> onMenuItemClicked;

    juce::StringArray getMenuBarNames() override
    {
        return { "File", "Edit" };
    }
    
    juce::PopupMenu getMenuForIndex(int menuIndex, const juce::String& menuName) override
    {
        juce::PopupMenu menu;

        if (menuName == "File")
        {
            menu.addItem("New", [this] { handleMenuItem("File", "New"); });
            menu.addItem("Open", [this] { handleMenuItem("File", "Open"); });
        }
        else if (menuName == "Edit")
        {
            menu.addItem("Undo", [this] { handleMenuItem("Edit", "Undo"); });
            menu.addItem("Redo", [this] { handleMenuItem("Edit", "Redo"); });
        }

        return menu;
    }

    void menuItemSelected(int /*menuItemID*/, int /*topLevelMenuIndex*/) override {}

private:
    void handleMenuItem(const juce::String& menu, const juce::String& item)
    {
        if (onMenuItemClicked)
            onMenuItemClicked(menu, item);
    }
};
