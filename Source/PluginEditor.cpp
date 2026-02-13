
#include "PluginProcessor.h"
#include "PluginEditor.h"

D1DelayAudioProcessorEditor::D1DelayAudioProcessorEditor (D1DelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), appGui(p)
{
    
    if (auto* mainDisplay = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay())
    {
        auto screenBounds = mainDisplay->userArea;
        screenWidth = screenBounds.getWidth();
        screenHeight = screenBounds.getHeight();
        
        float calcLoginHeight = screenHeight * .32;
        float calcAppHeight = screenHeight * .5;
        
        if(calcLoginHeight > loginWindowHeight)
        {
            loginWindowWidth = calcLoginHeight * loginWindowRatio;
            loginWindowHeight = calcLoginHeight;
        }
        
        if(calcAppHeight > loginWindowHeight)
        {
            windowWidth = calcAppHeight * ratio;
            windowHeight = calcAppHeight;
        }
        
    }
    
    juce::MemoryInputStream svgStream(BinaryData::logo_svg,
                                      BinaryData::logo_svgSize,
                                      false);
    juce::MemoryInputStream svgStreamPassVisOn(BinaryData::VisibilityOn_svg,
                                               BinaryData::VisibilityOn_svgSize,
                                      false);
    juce::MemoryInputStream svgStreamPassVisOff(BinaryData::VisibilityOff_svg,
                                                BinaryData::VisibilityOff_svgSize,
                                      false);
    

    juce::String logoContent = svgStream.readEntireStreamAsString();
    juce::String svgContentPassVisOn = svgStreamPassVisOn.readEntireStreamAsString();
    juce::String svgContentPassVisOff = svgStreamPassVisOff.readEntireStreamAsString();
    juce::XmlDocument logoSvgDoc(logoContent);
    juce::XmlDocument svgDocPassVisOn(svgContentPassVisOn);
    juce::XmlDocument svgDocPassVisOff(svgContentPassVisOff);
    
    std::unique_ptr<juce::XmlElement> logoSvgXml(logoSvgDoc.getDocumentElement());
    std::unique_ptr<juce::XmlElement> passVisOnSvgXml(svgDocPassVisOn.getDocumentElement());
    std::unique_ptr<juce::XmlElement> passVisOffSvgXml(svgDocPassVisOff.getDocumentElement());
       
    if (logoSvgXml != nullptr)
    {
        logo = juce::Drawable::createFromSVG(*logoSvgXml);
    }
    if (passVisOnSvgXml != nullptr)
    {
        passVisOn = juce::Drawable::createFromSVG(*passVisOnSvgXml);
    }
    if (passVisOffSvgXml != nullptr)
    {
        passVisOff = juce::Drawable::createFromSVG(*passVisOffSvgXml);
    }
    
    loginScreen.setAuthLock(authLock);
    loginScreen.setImages(logo, passVisOn, passVisOff);
    loginScreen.setLoginProcessCompleteCallback([this] { loginCompleteCallback(); });
    
    appGui.setLogoutButtonCallback([this] { logout(); });
    appGui.setAppName(audioProcessor.getProductName());
    
    if(audioProcessor.isLocked())
    {
        loginScreen.setVisible(true);
        appGui.setVisible(false);
    }
    else
    {
        loginScreen.setVisible(false);
        appGui.setVisible(true);
    }
    
    startTimer(1000/framerate);
    
    if(audioProcessor.isLocked())
    {
        setResizeLimits(loginWindowWidth*.75, loginWindowHeight*.75, loginWindowWidth*1.50, loginWindowHeight*1.50);
        getConstrainer()->setFixedAspectRatio(loginWindowWidth / loginWindowHeight);
        setSize(loginWindowWidth, loginWindowHeight);
    }
    else
    {
        setResizeLimits(200, 200/ratio, 1200, 1200/ratio);
        getConstrainer()->setFixedAspectRatio(ratio);
        setSize (windowWidth, windowHeight);
    }

    addAndMakeVisible(&appGui);
    addAndMakeVisible(&loginScreen);
}

D1DelayAudioProcessorEditor::~D1DelayAudioProcessorEditor()
{
}

void D1DelayAudioProcessorEditor::paint (juce::Graphics& g)
{

    
}

void D1DelayAudioProcessorEditor::resized()
{
    if(authLock->isLocked())
    {
        appGui.setBounds(0,0, getWidth(), getHeight());
        loginScreen.setBounds(0,0, getWidth(), getHeight());
    }
    else
    {
        appGui.setBounds(0,0,getWidth(),getHeight());
    }
}

void D1DelayAudioProcessorEditor::buttonClicked(juce::Button* button)
{

}

void D1DelayAudioProcessorEditor::loadLoginScreen()
{
    appGui.setVisible(false);
    loginScreen.setVisible(true);
    loginScreen.launchLoginScreen();
    loginScreen.setBounds(0,0,getWidth(),getHeight());
    loginScreen.logout();
    setResizable(true,false);
    setResizeLimits(loginWindowWidth*.75, loginWindowHeight*.75, loginWindowWidth*1.50, loginWindowHeight*1.50);
    getConstrainer()->setFixedAspectRatio(loginWindowWidth / loginWindowHeight);
    setSize(loginWindowWidth, loginWindowHeight);

}

void D1DelayAudioProcessorEditor::loadAppGui()
{
    loginScreen.setVisible(false);
    appGui.setVisible(true);
    setResizeLimits(200, 200/ratio, 1200, 1200/ratio);
    getConstrainer()->setFixedAspectRatio(ratio);
    setSize(windowWidth, windowHeight);
    appGui.setBounds(0, 0, getWidth(), getHeight());
    repaint();
}

void D1DelayAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{


}

void D1DelayAudioProcessorEditor::timerCallback()
{

    if(!loginScreen.loginComplete())
    {
        loginScreen.frameUpdate();
        repaint();
    }
    
    if(audioProcessor.getRepaintFlag())
    {
        repaint();
        audioProcessor.setRepaintFlag(false);
    }
    appGui.frameUpdate();
}


void D1DelayAudioProcessorEditor::logout()
{
    loadLoginScreen();
    authLock->logout();
    loginScreen.setBounds(0,0,getWidth(),getHeight());

}

void D1DelayAudioProcessorEditor::loginCompleteCallback()
    {
        loadAppGui();
    }
