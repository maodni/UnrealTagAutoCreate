#include <iostream>
#include <conio.h>
#include <filesystem>
#include <fstream>
#include <string>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif


namespace fs = std::filesystem;

namespace {
    using std::cout;
    using std::cin;
    using std::string;
}

int main() {
#ifdef _WIN32
    // 强制 Windows 控制台输入输出使用 UTF-8 编码
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif
    string inputTagConfig;
    bool readFile = false;
    {
        std::ifstream file("./data.json");
        string line;
        while (std::getline(file, line)) {
            inputTagConfig = line;
            if (!readFile) {
                readFile = true;
            }
        }
    }
    if (!readFile) {
        std::cout << "input tag config:" << "\n";
        cin >> inputTagConfig;
        cin.ignore();
    }

    if (!readFile) {
        std::ofstream file("./data.json");
        file << inputTagConfig;
    }

    // std::cout << "input head file:" << "\n";
    // string inputHeadFile;
    // cin >> inputHeadFile;
    // cin.ignore();
    // std::cout << "input cpp file:" << "\n";
    // string inputCppFile;
    // cin >> inputCppFile;
    // cin.ignore();

    fs::path tagPath = inputTagConfig;
    std::vector<string> tagsList;
    if (fs::exists(tagPath) && !fs::is_directory(tagPath)) {
        std::ifstream file(tagPath);
        string line;
        while (std::getline(file, line)) {
            std::cout << line << "\n";
        }
    }


    std::cout << "input any key exit..." << "\n";
    _getch();
    return 0;
}
