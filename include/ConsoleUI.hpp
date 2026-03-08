#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include <memory>
#include "ConfigManager.hpp"
#include "AccessController.hpp"
#include "ProcessMonitor.hpp"

namespace desklock {

class ConsoleUI {
public:
    // Pass in the config manager by reference so UI can modify the blocklist
    ConsoleUI(ConfigManager& configManager, AccessController& accessController);

    // Starts the main menu loop
    void run();

private:
    ConfigManager& m_configManager;
    AccessController& m_accessController;
    ProcessMonitor m_processMonitor;

    void displayMenu();

    void showBlockedApps();
    void addApp();
    void removeApp();
    void unlockApp(); // New option for Step 8
    void showRunningProcesses(); // New option for testing Step 5
    void clearScreen();
};

} // namespace desklock

#endif // CONSOLE_UI_H
