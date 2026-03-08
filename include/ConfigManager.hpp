#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>
#include <vector>

namespace desklock {

class ConfigManager {
public:
    // Load config from a JSON file. Creates a default one if it doesn't exist.
    bool loadConfig(const std::string& configFilePath);

    // Save current settings back to JSON.
    bool saveConfig(const std::string& configFilePath);

    // Get the configured password required to unlock apps.
    std::string getPassword() const;

    // Set a new password
    void setPassword(const std::string& newPassword);

    // Get the list of application names to be blocked (e.g. "chrome.exe")
    std::vector<std::string> getBlockedApps() const;

    // Add an app to block
    void addAppToBlocklist(const std::string& appName);

    // Remove an app from the blocklist
    void removeAppFromBlocklist(const std::string& appName);

private:
    std::string m_password = "admin"; // default fallback password
    std::vector<std::string> m_blockedApps;
};

} // namespace desklock

#endif // CONFIG_MANAGER_H
