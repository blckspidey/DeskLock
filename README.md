---

# 🔒 DeskLock

<div align="center">

### 🛡️ Lightweight Windows Application Blocker

A **console-based C++ security utility** that prevents selected applications from running by **monitoring processes and terminating unauthorized programs in real time**.

---

![C++](https://img.shields.io/badge/Language-C++-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-green)
![Build](https://img.shields.io/badge/Build-Makefile-orange)
![Status](https://img.shields.io/badge/Project-Active-success)
![License](https://img.shields.io/badge/License-Educational-lightgrey)

</div>

---

# ✨ Overview

**DeskLock** is a **lightweight Windows monitoring tool written in C++** that runs in the background and automatically blocks specific applications from running.

It continuously scans running processes and **terminates any application that appears in the blocklist**.

The application includes an **interactive CLI control panel**, password protection, and a persistent configuration system.

This project demonstrates practical **Windows systems programming concepts**, including:

* Process enumeration
* System-level process control
* Multithreading
* Secure CLI input
* File-based configuration management

---

# 🚀 Features

## 🔍 Real-Time Process Monitoring

Continuously scans running processes using the **Windows Tool Help API (`tlhelp32.h`)**.

## ❌ Automatic Process Termination

Blocked applications are immediately terminated using the **`TerminateProcess` Windows API**.

## 🖥️ Interactive Admin Control Panel

A simple **CLI menu system** allows administrators to manage blocked applications.

```
1. View Blocked Applications
2. Add Application to Blocklist
3. Remove Application
4. Temporary Unlock
5. Save and Exit
```

---

## 🔐 Password Protected Access

Administrative controls are protected using hidden input via **`conio.h`**.

Default password:

```
admin
```

---

## ⏳ Temporary Unlock Feature

Allows applications to run **temporarily**.

Example:

```
Allow chrome.exe for 5 minutes
```

After the timer expires, the app is **blocked again automatically**.

---

## 💾 Persistent JSON Configuration

DeskLock stores settings inside a JSON configuration file.

Saved data includes:

* Admin password
* Blocked applications list
* Temporary unlock timers

---

# 🛠️ Tech Stack

| Technology         | Purpose                          |
| ------------------ | -------------------------------- |
| **C++**            | Core programming language        |
| **Windows API**    | Process monitoring & termination |
| **tlhelp32.h**     | Process snapshot enumeration     |
| **conio.h**        | Hidden password input            |
| **JSON**           | Configuration storage            |
| **Multithreading** | Continuous monitoring            |

---

# 📦 Requirements

* Windows OS
* **MinGW (GCC)** or **MSVC Compiler**
* **Make / mingw32-make**

---

# ⚙️ Installation & Build

### 1️⃣ Clone the Repository

```bash
git clone https://github.com/yourusername/DeskLock.git
cd DeskLock
```

---

### 2️⃣ Build the Project

```bash
mingw32-make
```

---

### 3️⃣ Run the Application

```bash
.\build\DeskLock.exe
```

---

# 📖 Usage

### Step 1 — Start DeskLock

Run the executable.

---

### Step 2 — Login as Admin

Enter the default password:

```
admin
```

---

### Step 3 — Configure Blocklist

Example applications to block:

```
chrome.exe
cmd.exe
notepad.exe
discord.exe
```

---

### Step 4 — Save Configuration

Select:

```
Save and Exit
```

DeskLock will begin **monitoring and terminating blocked processes automatically**.

---

# 📂 Project Structure

```
DeskLock
│
├── src
│   ├── main.cpp
│   ├── process_monitor.cpp
│   ├── control_panel.cpp
│
├── build
│   └── DeskLock.exe
│
├── config.json
│
├── Makefile
│
└── README.md
```

---

# 🧠 How It Works

DeskLock follows this workflow:

```
Start Program
     │
     ▼
Load Config File
     │
     ▼
Start Background Monitor Thread
     │
     ▼
Scan Running Processes
     │
     ▼
Match Against Blocklist
     │
     ▼
Terminate Unauthorized Applications
```

This loop runs continuously to ensure blocked applications **cannot remain active**.

---

# ⚠️ Disclaimer

This project was created **for educational purposes only** to demonstrate:

* Windows process management
* C++ systems programming
* Background monitoring techniques

DeskLock **should not replace professional antivirus or enterprise endpoint management tools**.

---

# 🚧 Future Improvements

* 🖼 GUI Version
* 🔔 System Tray Background Service
* 📊 Activity Logging
* 🔑 Secure Password Hashing
* ⚡ Performance Optimization
* 🖥 Windows Startup Integration

---

# 👨‍💻 Author

Developed as a **C++ Systems Programming Project**.

---

<div align="center">

⭐ If you like this project, consider **starring the repository**!

</div>

---

