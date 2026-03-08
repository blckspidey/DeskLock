#include "ConfigManager.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

namespace desklock {

// A very basic JSON parser since the user's GCC compiler is too old
// for modern nlohmann/json.hpp
bool ConfigManager::loadConfig(const std::string& configFilePath) {
    std::ifstream inputFile(configFilePath);
    
    // If config file doesn't exist, let's create a default one
    if (!inputFile.is_open()) {
        std::cout << "[Config] No config found at " << configFilePath << ". Creating default config." << std::endl;
        saveConfig(configFilePath);
        return true;
    }

    std::string line;
    bool inBlockedApps = false;
    m_blockedApps.clear(); // Clear default apps before loading

    while (std::getline(inputFile, line)) {
        // Find password
        if (line.find("\"password\"") != std::string::npos) {
            size_t startQuote = line.find('"', line.find(':'));
            if (startQuote != std::string::npos) {
                size_t endQuote = line.find('"', startQuote + 1);
                if (endQuote != std::string::npos) {
                    m_password = line.substr(startQuote + 1, endQuote - startQuote - 1);
                }
            }
        }
        
        // Find blocked apps array
        if (line.find("\"blocked_apps\"") != std::string::npos) {
            inBlockedApps = true;
            continue;
        }

        // Parse items inside blocked_apps array
        if (inBlockedApps) {
            if (line.find(']') != std::string::npos) {
                inBlockedApps = false;
                continue;
            }

            size_t startQuote = line.find('"');
            if (startQuote != std::string::npos) {
                size_t endQuote = line.find('"', startQuote + 1);
                if (endQuote != std::string::npos) {
                    std::string appName = line.substr(startQuote + 1, endQuote - startQuote - 1);
                    m_blockedApps.push_back(appName);
                }
            }
        }
    }
    
    std::cout << "[Config] Loaded configuration successfully." << std::endl;
    return true;
}

bool ConfigManager::saveConfig(const std::string& configFilePath) {
    std::ofstream outputFile(configFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "[Config Error] Failed to write config to " << configFilePath << std::endl;
        return false;
    }

    // Write manual JSON to avoid external dependencies
    outputFile << "{\n";
    outputFile << "    \"password\": \"" << m_password << "\",\n";
    outputFile << "    \"blocked_apps\": [\n";

    for (size_t i = 0; i < m_blockedApps.size(); ++i) {
        outputFile << "        \"" << m_blockedApps[i] << "\"";
        if (i < m_blockedApps.size() - 1) {
            outputFile << ",";
        }
        outputFile << "\n";
    }

    outputFile << "    ]\n";
    outputFile << "}\n";
    
    outputFile.close();
    return true;
}

std::string ConfigManager::getPassword() const {
    return m_password;
}

void ConfigManager::setPassword(const std::string& newPassword) {
    m_password = newPassword;
}

std::vector<std::string> ConfigManager::getBlockedApps() const {
    return m_blockedApps;
}

void ConfigManager::addAppToBlocklist(const std::string& appName) {
    // Check if it already exists to avoid duplicates
    auto it = std::find(m_blockedApps.begin(), m_blockedApps.end(), appName);
    if (it == m_blockedApps.end()) {
        m_blockedApps.push_back(appName);
    }
}

void ConfigManager::removeAppFromBlocklist(const std::string& appName) {
    auto it = std::find(m_blockedApps.begin(), m_blockedApps.end(), appName);
    if (it != m_blockedApps.end()) {
        m_blockedApps.erase(it);
    }
}

} // namespace desklock
