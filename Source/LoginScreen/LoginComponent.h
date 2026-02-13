#pragma once
#include "../CustomLF.h"
class LoginComponent :  public juce::Component,
                        public juce::Button::Listener,
                        private juce::TextEditor::Listener
{

private:

    juce::String websiteURL = "";
    juce::URL websiteLink = juce::URL("");
    AuthenticationLock* authLock;
    bool animationComplete = false;

    std::shared_ptr<juce::Drawable> passVisOn;
    std::shared_ptr<juce::Drawable> passVisOff;

    juce::DrawableButton visibilityButton {"Visibility", juce::DrawableButton::ImageFitted};

    std::thread networkThread;

    std::function<void()> loginButtonCallback;

    LoginScreenBackgroundLogo backgroundLogo;
    
    LoginButtonLF loginButtonLF;
    juce::TextButton loginButton;

    juce::Label loginPagePrompt;
    
    juce::TextEditor emailField;
    juce::TextEditor passwordField;
    juce::Label emailLabel;
    juce::Label passwordLabel;
    juce::HyperlinkButton websiteLinkButton;
    VisOnOffImage passVisibleImage;
    juce::ImageButton passVisButton;
    bool passwordVisible = false;

    juce::Label emailAlertLabel;
    juce::Label passwordAlertLabel;

    TransparancyLayer blackLayer;
    juce::String userEmail = "";
    
    

public:

    LoginComponent()
    {

        addAndMakeVisible(backgroundLogo);

        addAndMakeVisible(visibilityButton);

        setWantsKeyboardFocus(true);

        blackLayer.setRGBA(0,0,0,100);
        addAndMakeVisible(blackLayer);
        loginPagePrompt.setText("Login to your account", juce::dontSendNotification);
        loginPagePrompt.setFont(juce::FontOptions(28.0f));
        loginPagePrompt.setColour(juce::Label::textColourId, juce::Colours::white);
        loginPagePrompt.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(loginPagePrompt);

        emailLabel.setText("Email:", juce::dontSendNotification);
        emailLabel.setFont(juce::FontOptions(16.0f));
        emailLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        emailLabel.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(emailLabel);

        emailField.setFont(juce::FontOptions(16.0f));
        emailField.addListener(this);
        emailField.setColour(juce::TextEditor::backgroundColourId, juce::Colour(35, 36, 37));
        emailField.setColour(juce::TextEditor::textColourId, juce::Colours::white);
        emailField.setColour(juce::TextEditor::outlineColourId, juce::Colours::grey);
        addAndMakeVisible(emailField);

        passwordLabel.setText("Password:", juce::dontSendNotification);
        passwordLabel.setFont(juce::FontOptions(16.0f));
        passwordLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        passwordLabel.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(passwordLabel);

        passwordField.setMultiLine(false);
        passwordField.setPasswordCharacter('*');
        passwordField.addListener(this);
        passwordField.setFont(juce::FontOptions(16.0f));
        passwordField.setColour(juce::TextEditor::backgroundColourId, juce::Colour(35, 36, 37));
        passwordField.setColour(juce::TextEditor::textColourId, juce::Colours::white);
        passwordField.setColour(juce::TextEditor::outlineColourId, juce::Colours::grey);
        addAndMakeVisible(passwordField);

        loginButton.setButtonText("Login");
        loginButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        loginButton.setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
        loginButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        loginButton.setLookAndFeel(&loginButtonLF);
        loginButton.addListener(this);
        
        //loginButton.setWantsKeyboardFocus(true);
        addAndMakeVisible(loginButton);
        

        websiteLinkButton.setButtonText(websiteURL);

        websiteLinkButton.setURL(websiteLink);
        websiteLinkButton.setFont(juce::FontOptions("Arial", 16.0f, juce::Font::bold), false, juce::Justification::centred);
        websiteLinkButton.setColour(juce::HyperlinkButton::textColourId, juce::Colour(157, 178, 191));

        websiteLinkButton.addListener(this);
        addAndMakeVisible(websiteLinkButton);

        passVisButton.setButtonText("Login");
        passVisButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        passVisButton.setColour(juce::TextButton::buttonColourId, juce::Colour(39, 55, 77));
        passVisButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        passVisButton.addListener(this);

        emailAlertLabel.setText("Please enter your email", juce::dontSendNotification);
        emailAlertLabel.setFont(juce::FontOptions(16.0f));
        emailAlertLabel.setColour(juce::Label::textColourId, juce::Colours::red);
        emailAlertLabel.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(emailAlertLabel);

        passwordAlertLabel.setText("Please enter your password", juce::dontSendNotification);
        passwordAlertLabel.setFont(juce::FontOptions(16.0f));
        passwordAlertLabel.setColour(juce::Label::textColourId, juce::Colours::red);
        passwordAlertLabel.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(passwordAlertLabel);


        addAndMakeVisible(&passVisibleImage);
        addAndMakeVisible(&passVisButton);
        emailAlertLabel.setVisible(false);
        passwordAlertLabel.setVisible(false);
        


    }

    std::string getEmail()
    {
        return emailField.getText().toStdString();
    }

    std::string getPassword()
    {
        return passwordField.getText().toStdString();
    }

    void setImages(std::shared_ptr<juce::Drawable> logo, std::shared_ptr<juce::Drawable> visOnImage, std::shared_ptr<juce::Drawable> visOffImage)
    {
        
        if (visOnImage == nullptr)
        {
            DBG("Null Pointer Received In Login Component");
        }
        
        backgroundLogo.setLogoImage(logo);
        passVisOn = visOnImage;
        passVisOff = visOffImage;
        passVisibleImage.setImages(visOnImage, visOffImage);
        
    }
    
    void autoLoadEmail()
    {
        std::string email = authLock->getUserEmail();
        emailField.setText(email);
    }

    void paint(juce::Graphics& g) override
    {

    }

    void resized() override
    {
        float loginButtonWidth = getWidth()*.30;
        float loginButtonHeight = getHeight()*.1;
        float textBoxWidth = getWidth() * .55;
        float textBoxHeight = getHeight() * .11;
        float labelWidth =  getWidth() * .2;
        float labelHeight = getHeight() *.1;
        float headerFontSize = getHeight() * .1;
        float labelFontSize = getHeight() * .06;
        juce::Font emailFont = juce::Font(juce::FontOptions("Arial", labelFontSize, juce::Font::bold));
        
        auto emailText = emailField.getText();
        auto passwordText = passwordField.getText();
        auto caretPosEmail = emailField.getCaretPosition();
        auto caretPosPassword = passwordField.getCaretPosition();
        
        backgroundLogo.setBounds(getBounds());
        blackLayer.setBounds(getBounds());
        loginPagePrompt.setFont(juce::FontOptions(headerFontSize));
        loginPagePrompt.setBounds(0,getHeight()*.1,getWidth(),getHeight()*.1);
        emailLabel.setFont(juce::FontOptions(labelFontSize));
        emailLabel.setBounds(getWidth()*.09,getHeight()*.26,labelWidth,labelHeight);
        emailField.setFont(juce::FontOptions(labelFontSize));
        emailField.setBounds(getWidth()*.5-textBoxWidth*.5,getHeight()*.26,textBoxWidth,textBoxHeight);
        emailField.setText("");
        emailField.setText(emailText);
        emailField.setCaretPosition(caretPosEmail);
    
        float emailAlertWidth = getWidth() * .6;
        float passwordAlertWidth = getWidth() * .6;
        
        passwordLabel.setFont(juce::FontOptions(labelFontSize));
        passwordLabel.setBounds(getWidth()*.03,getHeight()*.45,labelWidth,labelHeight);
        passwordField.setFont(juce::FontOptions(labelFontSize));
        passwordField.setBounds(getWidth()*.5-textBoxWidth*.5,getHeight()*.45,textBoxWidth,textBoxHeight);
        passwordField.setText("");
        passwordField.setText(passwordText);
        passwordField.setCaretPosition(caretPosPassword);
        
        emailAlertLabel.setFont(juce::FontOptions(labelFontSize * .9));
        emailAlertLabel.setBounds(getWidth()*.5 - emailAlertWidth/2, getHeight()*.34, emailAlertWidth, getHeight()*.1);
        passwordAlertLabel.setFont(juce::FontOptions(labelFontSize * .9));
        passwordAlertLabel.setBounds(getWidth()*.5 - passwordAlertWidth/2, getHeight()*.53, passwordAlertWidth, getHeight()*.1);

        
        loginButton.setBounds(getWidth()*.5 - loginButtonWidth*.5 ,getHeight()*.7,loginButtonWidth, loginButtonHeight);
        websiteLinkButton.setBounds(0, getHeight() - 30, getWidth(), 25);
        websiteLinkButton.setFont(juce::FontOptions("Arial", labelFontSize, juce::Font::bold), false, juce::Justification::centred);
        
        passVisibleImage.setBounds(getWidth()*.52 + textBoxWidth*.5, getHeight()*.46, getWidth()*.06,getWidth()*.06);
        passVisButton.setBounds(getWidth()*.52 + textBoxWidth*.5, getHeight()*.46, getWidth()*.06,getWidth()*.06);
        
    }

    bool keyPressed(const juce::KeyPress& key) override
    {
        if (key == juce::KeyPress::returnKey) // Check if Enter key was pressed
        {
            loginButton.triggerClick();
            return true;
        }
        return false;
    }
    
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override
    {
        loginButton.triggerClick(); // Simulate button click
    }


    void buttonClicked(juce::Button* button) override
    {
        if (button == &loginButton)
        {
            emailAlertLabel.setVisible(false);
            passwordAlertLabel.setVisible(false);

            std::string email = emailField.getText().toStdString();
            std::string password = passwordField.getText().toStdString();
            int atSymbolLocation = static_cast<int>(email.find("@"));

            bool okToSubmitRequest = true;

            if(email.size() > 320)
            {
                emailAlertLabel.setText("Please enter a valid email address", juce::NotificationType::dontSendNotification);
                emailAlertLabel.setVisible(true);
                okToSubmitRequest = false;
            }
            if(password.size() > 2500)
            {
                passwordAlertLabel.setText("Please enter a valid password", juce::NotificationType::dontSendNotification);
                passwordAlertLabel.setVisible(true);
                okToSubmitRequest = false;
            }
            if(atSymbolLocation < 0)
            {
                emailAlertLabel.setText("Please enter a valid email address", juce::NotificationType::dontSendNotification);
                emailAlertLabel.setVisible(true);
                okToSubmitRequest = false;
            }
            if(email.size() == 0)
            {
                emailAlertLabel.setText("Please enter your email address", juce::NotificationType::dontSendNotification);
                emailAlertLabel.setVisible(true);
                okToSubmitRequest = false;
            }
            if(password.size() == 0)
            {
                passwordAlertLabel.setText("Please enter your password", juce::NotificationType::dontSendNotification);
                passwordAlertLabel.setVisible(true);
                okToSubmitRequest = false;
            }

            if(okToSubmitRequest)
            {
                loginButtonCallback();
            }

        }
        if (button == & passVisButton)
        {
            if(passwordVisible)
            {
                passwordField.setPasswordCharacter('*');
                passwordVisible = false;
                passVisibleImage.switchToVisOn();
            }
            else
            {
                passwordField.setPasswordCharacter(0);
                passwordVisible = true;
                passVisibleImage.switchToVisOff();
            }
            grabKeyboardFocus();
        }
    };

    void setLoginButtonCallback(std::function<void()> newCallback)
    {
        loginButtonCallback = std::move(newCallback);
    }

    void frameUpdate()
    {

    }

    void setAuthLock(AuthenticationLock* authLock)
    {
        this->authLock = authLock;
    }

    void assignKeyboard()
    {
        grabKeyboardFocus();
    }
    
    void setUserEmail(juce::String userEmail)
    {
        this->userEmail = userEmail;
        emailField.setText(userEmail);
    }

};
