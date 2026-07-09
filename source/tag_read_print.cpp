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
    auto tempRes = G_ReplaceAll(info.tagName, ".", "_");
    auto leftIndex = 0;
    leftIndex = tempRes.find('_');
    auto rightIndex = 0;
    rightIndex = tempRes.find('_', leftIndex + 1);
    std::string tempStr = tempRes;
    if (leftIndex > 0 && rightIndex > 0) {
        tempStr = tempStr.substr(0, rightIndex);
    }
    auto pairIt = cacheTagsMap.find(tempStr);
    if (pairIt != cacheTagsMap.end()) {
        pairIt->second.push_back(info);
    } else {
        cacheTagsMap[tempStr].push_back(info);
    }
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
    int writeCount = 0;
    for (auto pair: cacheTagsMap) {
        if (tempTitle != pair.first) {
            tempTitle = pair.first;
            file << "\n//-------------";
            file << pair.first;
            file << "\n";
        }
        for (auto &it: pair.second) {
            auto tempStr = G_ReplaceAll(it.tagName, ".", "_");
            file << "UE_DECLARE_GAMEPLAY_TAG_EXTERN(" + tempStr + ")" + "\n";
            writeCount++;
        }
    }
    std::cout << "写入头文件: " << writeCount << " 个 " << "\n";
    std::cout.flush();
    file << "\n}" << "\n";
    file.close();
}

void TagReadPrint::PrintSourceFile() {
    std::ofstream file;
    file.open(outputSourceFilePath, std::ios::out | std::ios::ate | std::ios::trunc);;
    file << "#include \"" << G_GetHeadFileName(outputSourceFilePath) << ".h\"\n";
    file << "namespace GameplayTags {" << "\n";
    std::string tempTitle;
    int writeCount = 0;

    for (auto pair: cacheTagsMap) {
        if (tempTitle != pair.first) {
            tempTitle = pair.first;
            file << "\n//-------------";
            file << pair.first;
            file << "\n";
        }
        for (auto &it: pair.second) {
            auto tempStr = G_ReplaceAll(it.tagName, ".", "_");
            file << "UE_DEFINE_GAMEPLAY_TAG_COMMENT(" + tempStr + ", \"";
            file << it.tagName + "\" , \"" + it.tagComment + "\");" + "\n";

            writeCount++;
            std::cout << "写入源文件: " << writeCount << " 个 " << "\r";
        }
    }
    std::cout << "写入源文件: " << writeCount << " 个 " << "\n";
    std::cout.flush();
    file << "\n}" << "\n";
    file.close();
}
