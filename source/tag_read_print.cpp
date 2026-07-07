//
// Created by adn on 2026/7/7.
//

#include "tag_read_print.h"

#include <fstream>
#include <iostream>

namespace {
    using std::cin;
    using std::cout;
    using std::string;


    string G_ReplaceAll(std::string &str,
                        const std::string &oldValue,
                        const std::string &newValue) {
        size_t pos = 0;
        string result = str;
        while ((pos = result.find(oldValue, pos)) != std::string::npos) {
            result.replace(pos, oldValue.length(), newValue);
            pos += newValue.length();
        }
        return result;
    }

    std::string G_GetHeadFileName(std::string &str) {
        std::string tempStr;
        //排除private长度
        auto index = str.find("Private") + 7;
        //排除后缀
        auto index2 = str.size() - 4;
        if (index != std::string::npos) {
            tempStr = str.substr(index, index2 - index);
            size_t pos = 0;
            tempStr = G_ReplaceAll(tempStr, "\\\\", "/");
            tempStr = tempStr.substr(1, tempStr.size() - 1);
            return tempStr;
        }
    }
}

TagReadPrint::TagReadPrint() = default;

std::vector<TagInfo> TagReadPrint::GetTagsList() {
    return cacheTagsList;
}

void TagReadPrint::AddTag(TagInfo &info) {
    cacheTagsList.push_back(std::move(info));
}

std::string TagReadPrint::GetInputTagConfig() const {
    return inputTagConfig;
}

std::string TagReadPrint::GetOutputHeadFilePath() const {
    return outputHeadFilePath;
}

std::string TagReadPrint::GetOutputSourceFilePath() const {
    return outputSourceFilePath;
}

void TagReadPrint::SetOutputHeadFilePath(std::string &str) {
    outputHeadFilePath = std::move(str);
}

void TagReadPrint::SetOutputSourceFilePath(std::string &str) {
    outputSourceFilePath = std::move(str);
}

void TagReadPrint::SetInputTagConfig(std::string &str) {
    this->inputTagConfig = std::move(str);
}


void TagReadPrint::PrintHeadFile() {
    std::ofstream file;
    file.open(outputHeadFilePath, std::ios::out | std::ios::ate | std::ios::trunc);
    file << "#pragma once" << "\n";
    file << "#include \"CoreMinimal.h\"" << "\n";
    file << "#include \"NativeGameplayTags.h\"" << "\n";
    file << "namespace GameplayTags{" << "\n";
    std::string tempTitle = "";
    for (int i = 0; i < cacheTagsList.size(); ++i) {
        size_t pos = 0;
        std::string str = cacheTagsList[i].tagName;
        auto tempRes = G_ReplaceAll(str, ".", "_");
        auto leftIndex = tempRes.find('_');
        auto rightIndex = tempRes.rfind('_');
        std::string tempStr = tempRes.substr(leftIndex, rightIndex - leftIndex);
        if (tempTitle != tempStr) {
            tempTitle = tempStr;
            file << "\n//-------------";
            file << tempStr;
            file << "\n";
        }

        file << "UE_DECLARE_GAMEPLAY_TAG_EXTERN(" + tempRes + ")" + "\n";
    }
    file << "\n}" << "\n";
    file.close();
}

void TagReadPrint::PrintSourceFile() {
    std::ofstream file;
    file.open(outputSourceFilePath, std::ios::out | std::ios::ate | std::ios::trunc);;
    file << "#include \"" << G_GetHeadFileName(outputSourceFilePath) << ".h\"\n";
    file << "namespace GameplayTags {" << "\n";
    std::string tempTitle;

    for (int i = 0; i < cacheTagsList.size(); ++i) {
        std::string str = cacheTagsList[i].tagName;
        auto tempRes = G_ReplaceAll(str, ".", "_");
        auto leftIndex = str.find('.');
        auto leftSecondIndex = str.find('.', leftIndex);
        std::string tempStr = str.substr(0, leftSecondIndex);
        if (tempTitle != tempStr) {
            tempTitle = tempStr;
            file << "\n//-------------";
            file << tempStr;
            file << "\n";
        }

        file << "UE_DEFINE_GAMEPLAY_TAG_COMMENT(" + tempRes + ", \"";
        file << str + "\" , \"" + cacheTagsList[i].tagComment + "\");" + "\n";
        std::cout << "=======" << i << "/" << cacheTagsList.size() << "\r";
    }
    file << "\n}" << "\n";
    file.close();
}
