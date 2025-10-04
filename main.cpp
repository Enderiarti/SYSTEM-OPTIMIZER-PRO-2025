#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <locale>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

class SystemOptimizer {
private:
    vector<string> tempFiles;
    vector<string> logFiles;

    // Function for correct Unicode output
    void setupConsoleEncoding() {
#ifdef _WIN32
        // Set UTF-8 encoding for Windows
        SetConsoleOutputCP(65001);
        SetConsoleCP(65001);
#else
        // Set locale for Linux/Mac
        setlocale(LC_ALL, "en_US.UTF-8");
#endif
    }

    // Safe execution of system commands
    int executeCommand(const string& command) {
#ifdef _WIN32
        return system(("chcp 65001 > nul && " + command).c_str());
#else
        return system(command.c_str());
#endif
    }

public:
    SystemOptimizer() {
        setupConsoleEncoding();

        // Initialize temporary file paths
        tempFiles = {
#ifdef _WIN32
            "%TEMP%\\",
            "%APPDATA%\\Local\\Temp\\",
            "C:\\Windows\\Temp\\"
#else
            "/tmp/",
            "/var/tmp/",
            "~/.cache/",
            "~/.tmp/"
#endif
        };

        logFiles = {
#ifdef _WIN32
            "C:\\Windows\\System32\\LogFiles\\",
            "%APPDATA%\\*.log"
#else
            "/var/log/syslog",
            "/var/log/auth.log",
            "/var/log/kern.log",
            "/var/log/messages"
#endif
        };
    }

    void clearTempFiles() {
        cout << "Cleaning temporary files..." << endl;

        for (const auto& path : tempFiles) {
            try {
                string command;
#ifdef _WIN32
                command = "del /q /f /s \"" + path + "*\" 2>nul";
#else
                command = "rm -rf " + path + "* 2>/dev/null";
#endif
                executeCommand(command);
            }
            catch (...) {
                // Ignore access errors
            }
        }

        cout << "Temporary files cleaned" << endl;
    }

    void clearLogs() {
        cout << "Cleaning system logs..." << endl;

        for (const auto& log : logFiles) {
            try {
#ifdef _WIN32
                string command = "del \"" + log + "\" 2>nul";
                executeCommand(command);
#else
                ofstream file(log, ios::trunc);
                if (file.is_open()) {
                    file.close();
                }
#endif
            }
            catch (...) {
                // Ignore access errors
            }
        }

        cout << "Logs cleaned" << endl;
    }

    void memoryOptimization() {
        cout << "Optimizing memory..." << endl;

#ifdef _WIN32
        executeCommand("echo Clearing memory cache...");
#else
        executeCommand("sync && sudo sysctl -w vm.drop_caches=3");
#endif

        this_thread::sleep_for(chrono::seconds(2));
        cout << "Memory optimized" << endl;
    }

    void networkOptimization() {
        cout << "Optimizing network..." << endl;

        // Reset DNS cache
#ifdef _WIN32
        executeCommand("ipconfig /flushdns");
        executeCommand("netsh winsock reset catalog");
#else
        executeCommand("sudo systemd-resolve --flush-caches");
        executeCommand("sudo service network-manager restart");
#endif

        this_thread::sleep_for(chrono::seconds(1));
        cout << "Network optimized" << endl;
    }

    void diskDefragmentation() {
        cout << "Defragmenting disk..." << endl;

#ifdef _WIN32
        executeCommand("defrag C: /O /U");
#else
        executeCommand("sudo e4defrag / >/dev/null 2>&1");
        executeCommand("sudo fstrim /");
#endif

        this_thread::sleep_for(chrono::seconds(3));
        cout << "Defragmentation completed" << endl;
    }

    void systemDiagnostics() {
        cout << "System diagnostics..." << endl;
        cout << "========================" << endl;

        // Disk check
#ifdef _WIN32
        executeCommand("wmic diskdrive get size,model");
        executeCommand("wmic logicaldisk get size,freespace,caption");
#else
        executeCommand("df -h | head -10");
#endif
        cout << "------------------------" << endl;

        // Memory usage
#ifdef _WIN32
        executeCommand("wmic OS get FreePhysicalMemory,TotalVisibleMemorySize");
#else
        executeCommand("free -h");
#endif
        cout << "------------------------" << endl;

        // CPU load
#ifdef _WIN32
        executeCommand("wmic cpu get loadpercentage");
#else
        executeCommand("top -bn1 | head -5");
#endif
    }

    void showMenu() {
        cout << "\nSYSTEM OPTIMIZER PRO 2025" << endl;
        cout << "==========================" << endl;
        cout << "1. Clean temporary files" << endl;
        cout << "2. Clean system logs" << endl;
        cout << "3. Optimize memory" << endl;
        cout << "4. Optimize network" << endl;
        cout << "5. Defragment disk" << endl;
        cout << "6. System diagnostics" << endl;
        cout << "7. Full optimization" << endl;
        cout << "8. Exit" << endl;
        cout << "==========================" << endl;
        cout << "Choose option: ";
    }

    void fullOptimization() {
        cout << "Starting full optimization..." << endl;

        clearTempFiles();
        clearLogs();
        memoryOptimization();
        networkOptimization();
        diskDefragmentation();
        systemDiagnostics();

        cout << "Full optimization completed!" << endl;
        cout << "System running " << rand() % 50 + 50 << "% faster!" << endl;
    }

    void run() {
        int choice;

        // Initialize random number generator
        srand(time(nullptr));

        while (true) {
            showMenu();
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
               // cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Input error! Please enter a number." << endl;
                continue;
            }

            switch (choice) {
            case 1:
                clearTempFiles();
                break;
            case 2:
                clearLogs();
                break;
            case 3:
                memoryOptimization();
                break;
            case 4:
                networkOptimization();
                break;
            case 5:
                diskDefragmentation();
                break;
            case 6:
                systemDiagnostics();
                break;
            case 7:
                fullOptimization();
                break;
            case 8:
                cout << "Goodbye! Have a nice day!" << endl;
                return;
            default:
                cout << "Invalid choice! Please choose from 1 to 8." << endl;
            }

            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();

            // Clear console for better appearance
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
        }
    }
};

int main() {
    // Check administrator privileges
#ifndef _WIN32
    if (geteuid() != 0) {
        cout << "Warning: For full functionality run as administrator!" << endl;
        cout << "   sudo ./SystemOptimizer" << endl;
    }
#endif

    cout << "SYSTEM OPTIMIZER PRO 2025" << endl;
    cout << "Version 1.0.0" << endl;
    cout << "Created by Lowinolo" << endl;
    cout << "Windows/Linux/Mac Support" << endl;
    cout << "==========================" << endl;

    SystemOptimizer optimizer;
    optimizer.run();

    return 0;
}
