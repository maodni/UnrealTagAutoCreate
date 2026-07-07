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


    void G_ReplaceAll(std::string &str,
                      const std::string &oldValue,
                      const std::string &newValue) {
        size_t pos = 0;

        while ((pos = str.find(oldValue, pos)) != std::string::npos) {
            str.replace(pos, oldValue.length(), newValue);
            pos += newValue.length();
        }
    }

    std::string G_GetHeadFileName(std::string &str) {
        std::string tempStr;
        auto index1 = str.rfind("\\");
        auto index2 = str.rfind(".");
        tempStr = str.substr(index1 + 1, index2 - index1 - 1);
        return tempStr;
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
    file << "namespace GameplayTags{" << "\n";
    std::string tempTitle = "";
    for (int i = 0; i < cacheTagsList.size(); ++i) {
        size_t pos = 0;
        std::string str = cacheTagsList[i].tagName;
        G_ReplaceAll(str, ".", "_");
        auto leftIndex = str.find('_');
        auto rightIndex = str.rfind('_');
        std::string tempStr = str.substr(leftIndex, rightIndex - leftIndex);
        if (tempTitle != tempStr) {
            tempTitle = tempStr;
            file << "\n//-------------";
            file << tempStr;
            file << "\n";
        }

        file << "UE_DECLARE_GAMEPLAY_TAG_EXTERN(" + str + ");" + "\n";
    }
    file << "\n}" << "\n";
    file.close();
}

void TagReadPrint::PrintSourceFile() {
    std::ofstream file;
    file.open(outputSourceFilePath, std::ios::out | std::ios::ate | std::ios::trunc);;
    file << "#include \"" << G_GetHeadFileName(outputHeadFilePath) << ".cpp\"\n";
    file << "namespace GameplayTags {" << "\n";
    std::string tempTitle;

    for (int i = 0; i < cacheTagsList.size(); ++i) {
        size_t pos = 0;
        std::string str = cacheTagsList[i].tagName;
        G_ReplaceAll(str, ".", "_");
        auto leftIndex = str.find('_');
        auto rightIndex = str.rfind('_');
        std::string tempStr = str.substr(leftIndex, rightIndex - leftIndex);
        if (tempTitle != tempStr) {
            tempTitle = tempStr;
            file << "\n//-------------";
            file << tempStr;
            file << "\n";
        }

        file << "UE_DEFINE_GAMEPLAY_TAG_COMMENT(" + str + ", " + cacheTagsList[i].tagComment + ");" + "\n";
    }
    file << "\n}" << "\n";
    file.close();
}
