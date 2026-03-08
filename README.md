# DeskLock 🔒

DeskLock is a lightweight, console-based Windows application written in C++ that allows users to block specific applications from running. It runs as a background process monitor, detecting and terminating unauthorized applications while providing an interactive Control Panel for management.

## Features
- **Process Monitoring:** Continuously scans running Windows applications using the `<tlhelp32.h>` API.
- **Auto-Termination:** Forcefully kills matching applications using the native Windows `TerminateProcess` API.
- **Admin Control Panel:** A CLI menu to add or remove applications to the blocklist.
- **Password Protection:** Uses hidden inputs (`<conio.h>`) to require an administrator password to access settings.
- **Temporary Access:** Authorized users can temporarily bypass the lock (e.g. unlock Chrome for 5 minutes).
- **JSON Configuration:** Automatically saves and loads your blocklist and password to an external file.

## Requirements
- Windows Operating System
- MinGW (GCC compiler) or MSVC Compiler
- Make (or mingw32-make)

## How to Build
Navigate to the root directory and use the included Makefile:

```powershell
# Create the build folder and compile the code
mingw32-make

# Run the application
.\build\DeskLock.exe
```

## Usage
1. Run the application.
2. The default password is `admin`.
3. Use the Interactive Console Menu to add executables to the blocklist (e.g., `chrome.exe`, `cmd.exe`).
4. Select *Save and Exit* to apply the configuration.
5. The application will immediately begin monitoring and killing blocked processes in the background!

## Disclaimer
This project was developed for educational purposes to demonstrate systems programming concepts like C++ filesystem manipulation, OS threading, Process Snapshotting, and Window Handle manipulation. It should not replace enterprise antivirus or management software.
