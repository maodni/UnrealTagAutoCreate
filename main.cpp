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


struct TagInfo {
    string tagName;
    string tagComment;
    TagInfo() {}

    TagInfo(string &inTagName, string &inTagComment) : tagName(std::move(inTagName)
                                                       ), tagComment(std::move(inTagComment)) {}
};

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
    std::vector<TagInfo> tagsList;
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
                auto dotIndex = str.find(",");
                auto tagStr = str.substr(0, dotIndex);
                auto commentStr = str.substr(dotIndex + 1, str.size() - dotIndex);
                auto tagIndex = tagStr.find("=");
                auto tagRes = tagStr.substr(tagIndex + 1, tagStr.size() - tagIndex);
                // std::cout << tagRes << "\n";
                auto commentIndex = commentStr.find("=");
                auto commentRes = commentStr.substr(commentIndex + 1, commentStr.size() - commentIndex);
                // std::cout << commentRes << "\n";
                TagInfo info(tagRes, commentRes);
                tagsList.push_back(info);
            }
        }
    }
    for (const auto &pair: tagsList) {
        std::cout << pair.tagName << "---" << pair.tagComment << "\n";
    }

    std::cout << "input any key exit..." << "\n";
    _getch();
    return 0;
}
