#include <iostream>
#include <conio.h>
#include <filesystem>
#include <fstream>
#include <string>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
#include <nlohmann/json.hpp>
#include "source/tag_read_print.h"

namespace fs = std::filesystem;

namespace {
    using std::cout;
    using std::cin;
    using std::string;
    using nlohmann::json;
}


int main() {
#ifdef _WIN32
    // 强制 Windows 控制台输入输出使用 UTF-8 编码
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    string inputTagConfig;
    string outputHeadFilePath;
    string outputSourceFilePath;
    TagReadPrint trp;
    bool readFile = false;
    {
        std::ifstream file("./data.json");
        if (file.is_open()) {
            json j = json::parse(file);
            if (!j.empty()) {
                inputTagConfig = j["inputTagConfig"];
                outputHeadFilePath = j["outputHeadFilePath"];
                outputSourceFilePath = j["outputSourceFilePath"];
                trp.SetInputTagConfig(inputTagConfig);
                trp.SetOutputHeadFilePath(outputHeadFilePath);
                trp.SetOutputSourceFilePath(outputSourceFilePath);
                readFile = true;
            }
        }
    }
    std::cout << inputTagConfig << "\n";
    if (!readFile) {
        std::cout << "input tag config:" << "\n";
        cin >> inputTagConfig;
        trp.SetInputTagConfig(inputTagConfig);
        cin.ignore();
        std::cout << "input out Head file:" << "\n";
        cin >> outputHeadFilePath;
        trp.SetOutputHeadFilePath(outputHeadFilePath);
        cin.ignore();
        std::cout << "input out source file:" << "\n";
        cin >> outputSourceFilePath;
        trp.SetOutputSourceFilePath(outputSourceFilePath);
        cin.ignore();
    }

    if (!readFile) {
        // file << inputTagConfig;
        json j;
        j["inputTagConfig"] = trp.GetInputTagConfig();
        j["outputHeadFilePath"] = trp.GetOutputHeadFilePath();
        j["outputSourceFilePath"] = trp.GetOutputSourceFilePath();
        std::ofstream("data.json") << j.dump(4);
    }

    // std::cout << "input head file:" << "\n";
    // string inputHeadFile;
    // cin >> inputHeadFile;
    // cin.ignore();
    // std::cout << "input cpp file:" << "\n";
    // string inputCppFile;
    // cin >> inputCppFile;
    // cin.ignore();

    fs::path tagPath = trp.GetInputTagConfig();

    if (fs::exists(tagPath) && !fs::is_directory(tagPath)) {
        std::ifstream file(tagPath);
        string line;
        while (std::getline(file, line)) {
            if (line[0] == '+') {
                // std::cout << line << "\n";
                //拿到标签和说明内容
                auto leftIndex = line.find('(');
                auto rightIndex = line.find(')');
                auto str = line.substr(leftIndex + 1, rightIndex - leftIndex - 1);
                auto dotIndex = str.find(',');
                auto tagStr = str.substr(0, dotIndex);
                auto commentStr = str.substr(dotIndex + 1, str.size() - dotIndex);
                auto tagIndex = tagStr.find('=');
                auto tagRes = tagStr.substr(tagIndex + 1, tagStr.size() - tagIndex);
                // std::cout << tagRes << "\n";
                auto commentIndex = commentStr.find('=');
                auto commentRes = commentStr.substr(commentIndex + 1, commentStr.size() - commentIndex);
                // std::cout << commentRes << "\n";
                TagInfo info(tagRes, commentRes);
                trp.AddTag(info);
            }
        }
    }
    trp.PrintHeadFile();
    trp.PrintSourceFile();

    std::cout << "input any key exit..." << "\n";
    _getch();
    return 0;
}
