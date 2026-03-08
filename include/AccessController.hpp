#ifndef ACCESS_CONTROLLER_H
#define ACCESS_CONTROLLER_H

#include "ConfigManager.hpp"
#include <string>
#include <vector>
#include <chrono>

namespace desklock {

// Represents an application that has been temporarily unblocked
struct TempUnlock {
    std::string appName;
    std::chrono::time_point<std::chrono::steady_clock> unlockTime;
    int durationMinutes;
};

class AccessController {
public:
    AccessController(ConfigManager& configManager);

    // Prompt user for password in the console. Returns true if correct.
    bool promptForPassword();

    // Change the system password
    bool changePassword(const std::string& oldPassword, const std::string& newPassword);

    // Temporarily allow an application to run
    void temporarilyUnlockApp(const std::string& appName, int minutes);

    // Check if an application is currently allowed to run temporarily
    bool isAppTemporarilyUnlocked(const std::string& appName);

    // Clean up expired temporary unlocks
    void cleanupExpiredUnlocks();

private:
    ConfigManager& m_configManager;
    std::vector<TempUnlock> m_tempUnlocks;
    
    // Windows specific function to hide password typing
    std::string getHiddenInput();
};

} // namespace desklock

#endif // ACCESS_CONTROLLER_H
