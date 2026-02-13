#pragma once
#include <iostream>
#include <thread>
#include <JuceHeader.h>

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
    void lock(){locked.store(true);};
    void unlock(){locked.store(false);};
    
    
private:
    
    std::atomic<bool> locked = false;
    std::function<void()> loginRequestCallback;
    std::string userEmail = "";
   
    int authStatusCode = 200;
    int loginStatusCode = 200;
    std::string loginMessage = "";
    std::string errorMessage = "";

    
};


