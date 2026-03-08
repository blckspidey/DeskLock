#include "ConsoleUI.hpp"
#include <iostream>
#include <string>

namespace desklock {

ConsoleUI::ConsoleUI(ConfigManager& configManager, AccessController& accessController) 
    : m_configManager(configManager), m_accessController(accessController) {
}

void ConsoleUI::clearScreen() {
#ifdef _WIN32
    system("cls"); // Windows clear screen
#else
    system("clear"); // Linux/Mac clear screen
#endif
}

void ConsoleUI::displayMenu() {
    std::cout << "========================================\n";
    std::cout << "          DeskLock Control Panel        \n";
    std::cout << "========================================\n";
    std::cout << "[1] View Blocked Applications\n";
    std::cout << "[2] Add Application to Blocklist\n";
    std::cout << "[3] Remove Application from Blocklist\n";
    std::cout << "[4] Temporarily Unlock Application\n";
    std::cout << "[5] View Running Processes (Test)\n";
    std::cout << "[6] Save and Exit\n";
    std::cout << "========================================\n";
    std::cout << "Select an option: ";
}

void ConsoleUI::showBlockedApps() {
    clearScreen();
    std::cout << "--- Current Blocked Applications ---\n\n";
    
    auto apps = m_configManager.getBlockedApps();
    if (apps.empty()) {
        std::cout << "   (No applications are currently blocked)\n";
    } else {
        for (size_t i = 0; i < apps.size(); ++i) {
            std::cout << " " << i + 1 << ". " << apps[i] << "\n";
        }
    }
    
    std::cout << "\nPress Enter to return to menu...";
    std::cin.ignore();
    std::cin.get();
}

void ConsoleUI::addApp() {
    clearScreen();
    std::cout << "--- Add Application to Blocklist ---\n";
    std::cout << "Enter the exact executable name (e.g., chrome.exe)\n";
    std::cout << "App Name: ";
    
    std::string appName;
    std::cin >> appName;
    
    m_configManager.addAppToBlocklist(appName);
    std::cout << "\n[Success] Added '" << appName << "' to blocklist.\n";
    
    std::cout << "\nPress Enter to return to menu...";
    std::cin.ignore();
    std::cin.get();
}

void ConsoleUI::removeApp() {
    clearScreen();
    std::cout << "--- Remove Application ---\n";
    showBlockedApps(); // This will clear screen again and pause, but it's okay for our simple UI
    
    std::cout << "\nEnter the exact executable name to remove: ";
    std::string appName;
    std::cin >> appName;
    
    m_configManager.removeAppFromBlocklist(appName);
    std::cout << "\n[Success] Removed '" << appName << "' from blocklist (if it existed).\n";
    
    std::cout << "\nPress Enter to return to menu...";
    std::cin.ignore();
    std::cin.get();
}

void ConsoleUI::unlockApp() {
    clearScreen();
    std::cout << "--- Temporarily Unlock Application ---\n";
    showBlockedApps(); // Show what's available
    
    std::cout << "\nEnter the exact executable name to unlock (e.g., chrome.exe): ";
    std::string appName;
    std::cin >> appName;
    
    std::cout << "Enter duration in minutes: ";
    int minutes;
    if (!(std::cin >> minutes) || minutes <= 0) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "[Error] Invalid duration.\n";
    } else {
        std::cout << "Please verify your password to unlock: ";
        if (m_accessController.promptForPassword()) {
            m_accessController.temporarilyUnlockApp(appName, minutes);
        } else {
            std::cout << "[Security] Unlock denied.\n";
        }
    }
    
    std::cout << "\nPress Enter to return to menu...";
    std::cin.ignore();
    std::cin.get();
}

void ConsoleUI::showRunningProcesses() {
    clearScreen();
    std::cout << "--- Currently Running Processes (Test) ---\n\n";
    
    auto processes = m_processMonitor.getRunningProcesses();
    std::cout << "Found " << processes.size() << " processes.\n";
    
    // Only show the first 20 to avoid flooding the console
    int limit = processes.size() > 20 ? 20 : processes.size();
    for (int i = 0; i < limit; ++i) {
        std::cout << " PID: " << processes[i].id << " | Name: " << processes[i].name << "\n";
    }
    if (processes.size() > 20) {
        std::cout << " ... (and " << processes.size() - 20 << " more)\n";
    }
    
    std::cout << "\nPress Enter to return to menu...";
    std::cin.ignore();
    std::cin.get();
}

void ConsoleUI::run() {
    clearScreen();
    std::cout << "--- DeskLock Admin Login ---\n";
    
    // Demand password before showing menu
    while (!m_accessController.promptForPassword()) {
        std::cout << "Please try again.\n";
    }

    bool running = true;
    int choice = 0;

    while (running) {
        clearScreen();
        displayMenu();

        if (!(std::cin >> choice)) {
            // Clear error flags and discard bad input if user types letters instead of numbers
            std::cin.clear();
            std::cin.ignore(10000, '\n'); 
            continue; 
        }

        switch (choice) {
            case 1:
                showBlockedApps();
                break;
            case 2:
                addApp();
                break;
            case 3:
                removeApp();
                break;
            case 4:
                unlockApp();
                break;
            case 5:
                showRunningProcesses();
                break;
            case 6:
                std::cout << "\nSaving configuration...\n";
                m_configManager.saveConfig("config/config.json");
                std::cout << "Exiting DeskLock Control Panel. Goodbye!\n";
                running = false;
                break;
            default:
                std::cout << "\nInvalid choice. Please press Enter and try again.";
                std::cin.ignore();
                std::cin.get();
                break;
        }
    }
}

} // namespace desklock
