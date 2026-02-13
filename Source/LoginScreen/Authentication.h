#pragma once
#include <iostream>
#include <thread>
#include <JuceHeader.h>


// Public repo note:
// The production version includes a commercial licensing/auth system.
// This repository uses a local stub so the demo project builds and runs without network dependencies.
// To use the authentication system, you will add your logic to this class. The "locked" variable is what controls whether the app runs or goes to the login page.


class AuthenticationLock
{
    
public:
    
    AuthenticationLock(){};
    ~AuthenticationLock(){};
    
    void initialize(){};
    int getLoginStatusCode(){return loginStatusCode;};
    std::string getUserEmail(){return userEmail;};
    bool isLocked(){return locked.load();};
    std::string getErrorMessage(){return errorMessage;};
    void loginThread(std::string email, std::string password){};
    void logout(){lock();};
    void setLoginRequestCallback(std::function<void()> newCallback){loginRequestCallback = std::move(newCallback);};
    
    // fire this function to lock the app
    void lock(){locked.store(true);};
    
    //fire this function to unlock the app after your authentication process is successful.
    void unlock(){locked.store(false);};
    
    
private:
    
    //set to unlocked by default for demo purposes, this is normally set to true to lock the app automatically until the user has been authenticated
    std::atomic<bool> locked = false;
    
    
    std::function<void()> loginRequestCallback;
    std::string userEmail = "";
   
    int authStatusCode = 200;
    int loginStatusCode = 200;
    std::string loginMessage = "";
    std::string errorMessage = "";

    
};


