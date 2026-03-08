#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H

#include <string>
#include <vector>

// Forward declaration of the Process struct
namespace desklock {

struct Process {
    unsigned long id;
    std::string name;
};

class ProcessMonitor {
public:
    ProcessMonitor();
    ~ProcessMonitor(); // Destructor to clean up resources if needed

    // Retrieve a list of all currently running processes in Windows
    std::vector<Process> getRunningProcesses();

    // Kill a specific process by its ID
    bool terminateProcess(unsigned long processId);

    // Checks if the given process name matches any of the blocked apps
    bool isAppBlocked(const std::string& processName, const std::vector<std::string>& blocklist);

private:
    // Helper function to convert Windows wide strings to standard strings
    std::string convertWideString(const wchar_t* wstr);
};

} // namespace desklock

#endif // PROCESS_MONITOR_H
