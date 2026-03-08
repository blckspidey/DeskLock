#include "ConfigManager.hpp"
#include "ConsoleUI.hpp"
#include "AccessController.hpp"
#include "ProcessMonitor.hpp"
#include <windows.h>
#include <iostream>

using namespace desklock;

// Struct to pass arguments to the Windows Thread
struct ThreadArgs {
    ConfigManager* config;
    AccessController* access;
};

// This function will run in the background constantly checking for blocked apps
DWORD WINAPI BackgroundMonitor(LPVOID lpParam) {
    ThreadArgs* args = static_cast<ThreadArgs*>(lpParam);
    ProcessMonitor monitor;
    
    while (true) {
        // Sleep for 2 seconds
        Sleep(2000);
        
        auto blocklist = args->config->getBlockedApps();
        if (blocklist.empty()) continue; // Nothing to check
        
        auto runningProcesses = monitor.getRunningProcesses();
        
        for (const auto& process : runningProcesses) {
            if (monitor.isAppBlocked(process.name, blocklist)) {
                // If the app is temporarily unlocked, don't kill it!
                if (!args->access->isAppTemporarilyUnlocked(process.name)) {
                    // KILL IT!
                    if (monitor.terminateProcess(process.id)) {
                        std::cout << "\n\n[Security] Terminated blocked application: " << process.name << "\n";
                        std::cout << "Select an option: "; // Restore the console prompt visually
                    }
                }
            }
        }
    }
    return 0;
}

int main() {
    // 1. Create instances of core modules
    ConfigManager configManager;
    AccessController accessController(configManager);
    
    // 2. Load the configuration file at startup
    std::string configPath = "config/config.json";
    configManager.loadConfig(configPath);
    
    // 3. Start the background monitoring thread using Windows API
    ThreadArgs* args = new ThreadArgs{&configManager, &accessController};
    HANDLE hThread = CreateThread(NULL, 0, BackgroundMonitor, args, 0, NULL);
    if (hThread) {
        CloseHandle(hThread); // We don't need the handle anymore, the thread runs detached
    }

    // 4. Start the Console Control Panel with the loaded configuration
    ConsoleUI controlPanel(configManager, accessController);
    controlPanel.run(); // This acts as an infinite loop until the user exits

    return 0;
}
