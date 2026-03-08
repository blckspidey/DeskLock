#include "ProcessMonitor.hpp"

// Windows API headers
#include <windows.h>
#include <tlhelp32.h> // Tool Help Library for scanning processes
#include <iostream>
#include <algorithm>

namespace desklock {

ProcessMonitor::ProcessMonitor() {
}

ProcessMonitor::~ProcessMonitor() {
}

// Convert from Windows WCHAR to standard C++ string
std::string ProcessMonitor::convertWideString(const wchar_t* wstr) {
    if (wstr == nullptr) return std::string();
    
    // Calculate string length
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    // Convert wide char to utf-8 string
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &strTo[0], size_needed, NULL, NULL);
    
    // Remove the null terminator that gets copied over
    if (!strTo.empty() && strTo.back() == '\0') {
        strTo.pop_back();
    }
    return strTo;
}

std::vector<Process> ProcessMonitor::getRunningProcesses() {
    std::vector<Process> processList;

    // Create a snapshot of all processes running in the system
    // TH32CS_SNAPPROCESS tells the OS we want a list of processes
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    // If the snapshot failed (e.g., no permissions), return empty list
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "[Error] Failed to create process snapshot." << std::endl;
        return processList;
    }

    // PROCESSENTRY32 is a Windows struct that holds information about a single process
    PROCESSENTRY32 pe32;
    // We MUST set the dwSize inside the struct before calling Process32First!
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Get the first process in the snapshot
    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap); // Clean up
        std::cerr << "[Error] Failed to fetch first process." << std::endl;
        return processList;
    }

    // Loop through all processes
    do {
        Process p;
        p.id = pe32.th32ProcessID;

#ifdef UNICODE
        // If compiling natively with UNICODE enabled, szExeFile is a wchar_t array
        p.name = convertWideString(pe32.szExeFile);
#else
        // If not using UNICODE, szExeFile is just a char array
        p.name = pe32.szExeFile;
#endif
        
        // Convert process name to lowercase for easier string comparison
        std::transform(p.name.begin(), p.name.end(), p.name.begin(),
            [](unsigned char c){ return std::tolower(c); });

        processList.push_back(p);

    } while (Process32Next(hProcessSnap, &pe32)); // Get the next process

    // Always close theHANDLE when done to prevent memory leaks!
    CloseHandle(hProcessSnap);
    
    return processList;
}

bool ProcessMonitor::terminateProcess(unsigned long processId) {
    // Open the process with termination rights
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
    
    if (hProcess == NULL) {
        // We might not have permission to close this process
        return false;
    }

    // Try to forcefully terminate the process
    bool success = TerminateProcess(hProcess, 1);
    
    // Always close the handle
    CloseHandle(hProcess);
    
    return success;
}

bool ProcessMonitor::isAppBlocked(const std::string& processName, const std::vector<std::string>& blocklist) {
    // Both processName and items in blocklist should be lowercase for comparison
    // processName is already lowercase from getRunningProcesses()
    
    for (const auto& blockedApp : blocklist) {
        std::string lowerBlocked = blockedApp;
        std::transform(lowerBlocked.begin(), lowerBlocked.end(), lowerBlocked.begin(),
            [](unsigned char c){ return std::tolower(c); });
            
        if (processName == lowerBlocked) {
            return true;
        }
    }
    return false;
}

} // namespace desklock
