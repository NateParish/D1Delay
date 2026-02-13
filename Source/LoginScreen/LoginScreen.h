#pragma once

#include <JuceHeader.h>
#include <string>
#include <vector>
#include "Authentication.h"
#include "LoginScreenBackgroundLogo.h"
#include "TransparancyLayer.h"
#include "VisOnOffImage.h"
#include "LoginComponent.h"
#include "WelcomeComponent.h"
#include "LoadingComponent.h"
#include "ErrorComponent.h"
#include "NoLicense.h"
#include "LicenseExpired.h"
#include "TooManyInstalls.h"


class LoginScreen : public juce::Component,
                    public juce::Button::Listener,
                    private juce::TextEditor::Listener
{
    
private:
    
    juce::String websiteURL = "";
    juce::URL websiteLink = juce::URL("");
    AuthenticationLock* authLock;
    std::atomic<bool> loginProcessComplete = false;
    std::atomic<bool> waitingForLoginRequest = false;
   // juce::String userEmail;
    
    std::function<void()> loginProcessCompleteCallback;
    std::shared_ptr<juce::Drawable> logo;
    std::shared_ptr<juce::Drawable> visOnImage;
    std::shared_ptr<juce::Drawable> visOffImage;
    
    
    LoginComponent login;
    WelcomeComponent welcome;
    LoadingComponent loading;
    ErrorMessageComponent error;
    NoLicenseComponent noLicense;
    SubscriptionExpiredComponent subscriptionExpired;
    TooManyInstallsComponent tooManyInstalls;
    

public:
    LoginScreen()
    {
        addAndMakeVisible(&login);
        addAndMakeVisible(&welcome);
        addAndMakeVisible(&loading);
        addAndMakeVisible(&error);
        addAndMakeVisible(&noLicense);
        addAndMakeVisible(&subscriptionExpired);
        addAndMakeVisible(&tooManyInstalls);
        login.setLoginButtonCallback([this] { loginButtonCallback(); });
        login.setAuthLock(authLock);
        loading.setCancelButtonCallback([this]{ cancelButtonCallback() ;});
        error.setOkButtonCallback([this]{ okButtonCallback() ;});
        noLicense.setOkButtonCallback([this]{ okButtonCallback() ;});
        subscriptionExpired.setOkButtonCallback([this]{ okButtonCallback() ;});
        tooManyInstalls.setOkButtonCallback([this]{ okButtonCallback() ;});
        welcome.setAnimationCompleteCallback([this]{ animationCompleteCallback() ;});
        
        loadFirstScreen();
    }
    
    bool loginComplete()
    {
        return loginProcessComplete.load();
    }
    
    void logout()
    {
        loginProcessComplete.store(false);
    }
    
    
    void setAuthLock(AuthenticationLock* authLock)
    {
        this->authLock = authLock;
        login.setAuthLock(authLock);
        error.setAuthLock(authLock);
        noLicense.setAuthLock(authLock);
        authLock->setLoginRequestCallback([this]{ loginRequestCallback() ;});
        login.setUserEmail(authLock->getUserEmail());
    };
    
    void setImages(std::shared_ptr<juce::Drawable> logo, std::shared_ptr<juce::Drawable> passVisOn, std::shared_ptr<juce::Drawable> passVisOff)
    {
        if (passVisOn == nullptr)
        {
            DBG("Null Pointer Received - LOGIN SCREEN");
        }
        this->logo = logo;
        loading.setLogoImage(logo);
        welcome.setLogoImage(logo);
        error.setLogoImage(logo);
        noLicense.setLogoImage(logo);
        subscriptionExpired.setLogoImage(logo);
        tooManyInstalls.setLogoImage(logo);
        login.setImages(logo, passVisOn, passVisOff);
        
    }
    
    void autoLoadEmail()
    {
       // login.autoLoadEmail();
      //  DBG("auto loaded email fired...");
    }
    
    
    void paint(juce::Graphics& g) override
    {

    }
    
    void resized() override
    {
        login.setBounds(0,0,getWidth(),getHeight());
        welcome.setBounds(0,0,getWidth(),getHeight());
        loading.setBounds(0,0,getWidth(),getHeight());
        error.setBounds(0,0,getWidth(),getHeight());
        noLicense.setBounds(0,0,getWidth(),getHeight());
        subscriptionExpired.setBounds(0,0,getWidth(),getHeight());
        tooManyInstalls.setBounds(0,0,getWidth(),getHeight());
    }
    
    void buttonClicked(juce::Button* button) override
    {

    }
    
    void loginButtonCallback()
        {
            loginProcessComplete.store(false);
            waitingForLoginRequest.store(true);
            authLock->loginThread(login.getEmail(),login.getPassword());
            launchLoadingScreen();
        }
    
    void cancelButtonCallback()
        {
            waitingForLoginRequest.store(false);
            launchLoginScreen();
        }

    void okButtonCallback()
        {
            launchLoginScreen();
        }
    
    void loginRequestCallback()
    {

    }
    
    void animationCompleteCallback()
    {
        //animationComplete = true;
        loginProcessCompleteCallback();
    }
    
    void setLoginProcessCompleteCallback(std::function<void()> newCallback)
    {
        loginProcessComplete.store(true);
        loginProcessCompleteCallback = std::move(newCallback);
    }
    
    void frameUpdate()
    {
        if(waitingForLoginRequest.load())
        {
            int statusCode = authLock->getLoginStatusCode();
            std::string errorMessage = authLock->getErrorMessage();
//            DBG(statusCode);
            if(statusCode == 0)
            {
                //DBG("ZERO");
            }
            else if(statusCode == 200)
            {
                waitingForLoginRequest.store(false);
                launchWelcomeScreen();
            }
            else if(statusCode == 402)
            {
                waitingForLoginRequest.store(false);
                //noLicense.loadMessages();
                launchNeedLicenseScreen();
            }
            else if(errorMessage == "subscription expired")
            {
                waitingForLoginRequest.store(false);
                //noLicense.loadMessages();
                launchSubscriptionExpiredScreen();
            }
            else if(errorMessage == "too many installs")
            {
                waitingForLoginRequest.store(false);
                //noLicense.loadMessages();
                launchTooManyInstallsScreen();
            }
            
            else
            {
                waitingForLoginRequest.store(false);
                error.loadMessages();
                launchErrorScreen();
            }
        }
        
        welcome.frameUpdate();

        
    }
    
    void loadFirstScreen()
    {
        login.setVisible(true);
        loading.setVisible(false);
        welcome.setVisible(false);
        error.setVisible(false);
        noLicense.setVisible(false);
        subscriptionExpired.setVisible(false);
        tooManyInstalls.setVisible(false);
        repaint();
    }
    
    void launchLoginScreen()
    {
        login.setVisible(true);
        loading.setVisible(false);
        welcome.setVisible(false);
        error.setVisible(false);
        noLicense.setVisible(false);
        subscriptionExpired.setVisible(false);
        tooManyInstalls.setVisible(false);
        login.assignKeyboard();
        repaint();
        
    }
    void launchLoadingScreen()
    {
        login.setVisible(false);
        loading.setVisible(true);
        welcome.setVisible(false);
        error.setVisible(false);
        noLicense.setVisible(false);
        subscriptionExpired.setVisible(false);
        tooManyInstalls.setVisible(false);
        loading.assignKeyboard();
        
    }
    void launchWelcomeScreen()
    {
        login.setVisible(false);
        loading.setVisible(false);
        welcome.setVisible(true);
        error.setVisible(false);
        noLicense.setVisible(false);
        subscriptionExpired.setVisible(false);
        tooManyInstalls.setVisible(false);
        welcome.assignKeyboard();
        welcome.startAnimation();
    }
    void launchErrorScreen()
    {
        login.setVisible(false);
        loading.setVisible(false);
        welcome.setVisible(false);
        error.setVisible(true);
        noLicense.setVisible(false);
        subscriptionExpired.setVisible(false);
        tooManyInstalls.setVisible(false);
        error.assignKeyboard();
    }
    
    void launchNeedLicenseScreen()
    {
        login.setVisible(false);
        loading.setVisible(false);
        welcome.setVisible(false);
        error.setVisible(false);
        noLicense.setVisible(true);
        subscriptionExpired.setVisible(false);
        tooManyInstalls.setVisible(false);
        noLicense.assignKeyboard();
        repaint();
    }
    
    void launchSubscriptionExpiredScreen()
    {
        login.setVisible(false);
        loading.setVisible(false);
        welcome.setVisible(false);
        error.setVisible(false);
        noLicense.setVisible(false);
        subscriptionExpired.setVisible(true);
        tooManyInstalls.setVisible(false);
        subscriptionExpired.assignKeyboard();
        repaint();
    }
    
    void launchTooManyInstallsScreen()
    {
        login.setVisible(false);
        loading.setVisible(false);
        welcome.setVisible(false);
        error.setVisible(false);
        noLicense.setVisible(false);
        subscriptionExpired.setVisible(false);
        tooManyInstalls.setVisible(true);
        tooManyInstalls.assignKeyboard();
        repaint();
    }
};









