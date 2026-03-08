#include "AccessController.hpp"
#include <iostream>
#include <algorithm>

#ifdef _WIN32
#include <conio.h> // For _getch() on Windows
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace desklock {

AccessController::AccessController(ConfigManager& configManager) 
    : m_configManager(configManager) {
}

// Function to read password without showing characters on screen
std::string AccessController::getHiddenInput() {
    std::string password;
#ifdef _WIN32
    char ch;
    while ((ch = _getch()) != '\r') { // Enter key is \r on Windows
        if (ch == '\b') { // Backspace
            if (!password.empty()) {
                std::cout << "\b \b";
                password.pop_back();
            }
        } else if (ch != '\n') {
            password.push_back(ch);
            std::cout << '*';
        }
    }
    std::cout << std::endl;
#else
    // Basic fallback for non-Windows (though this is a Windows app)
    std::cin >> password; 
#endif
    return password;
}

bool AccessController::promptForPassword() {
    std::cout << "Authentication Required.\n";
    std::cout << "Enter Password: ";
    
    std::string input = getHiddenInput();
    
    if (input == m_configManager.getPassword()) {
        return true;
    }
    
    std::cout << "[Error] Incorrect Password!\n";
    return false;
}

bool AccessController::changePassword(const std::string& oldPassword, const std::string& newPassword) {
    if (oldPassword == m_configManager.getPassword()) {
        m_configManager.setPassword(newPassword);
        return true;
    }
    return false;
}

void AccessController::temporarilyUnlockApp(const std::string& appName, int minutes) {
    TempUnlock unlock;
    unlock.appName = appName;
    unlock.unlockTime = std::chrono::steady_clock::now();
    unlock.durationMinutes = minutes;
    
    m_tempUnlocks.push_back(unlock);
    std::cout << "[Info] " << appName << " is unlocked for " << minutes << " minutes.\n";
}

bool AccessController::isAppTemporarilyUnlocked(const std::string& appName) {
    cleanupExpiredUnlocks(); // Clean before checking
    
    for (const auto& unlock : m_tempUnlocks) {
        if (unlock.appName == appName) {
            return true;
        }
    }
    return false;
}

void AccessController::cleanupExpiredUnlocks() {
    auto now = std::chrono::steady_clock::now();
    
    // Remove all unlocks where the current time is greater than (unlockTime + duration)
    m_tempUnlocks.erase(
        std::remove_if(m_tempUnlocks.begin(), m_tempUnlocks.end(), 
            [&now](const TempUnlock& unlock) {
                auto expirationTime = unlock.unlockTime + std::chrono::minutes(unlock.durationMinutes);
                return now > expirationTime;
            }), 
        m_tempUnlocks.end()
    );
}

} // namespace desklock
