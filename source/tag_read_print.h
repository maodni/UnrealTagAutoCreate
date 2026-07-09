//
// Created by adn on 2026/7/7.
//

#ifndef UNREALTAGAUTOCREATE_TAG_READ_PRINT_H
#define UNREALTAGAUTOCREATE_TAG_READ_PRINT_H
#include <map>
#include <string>
#include <vector>

struct TagInfo {
    std::string tagName;
    std::string tagComment;

    TagInfo() = default;

    TagInfo(std::string &inTagName, std::string &inTagComment) : tagName(std::move(inTagName)
                                                                 ), tagComment(std::move(inTagComment)) {}
};

class TagReadPrint {
public:
    TagReadPrint();

    std::vector<TagInfo> GetTagsList();

    void AddTag(TagInfo &info);

    std::string GetInputTagConfig() const;

    std::string GetOutputHeadFilePath() const;

    std::string GetOutputSourceFilePath() const;

    void SetInputTagConfig(std::string &str);

    void SetOutputHeadFilePath(std::string &str);

    void SetOutputSourceFilePath(std::string &str);

    void PrintHeadFile();

    void PrintSourceFile();

private:
    std::string inputTagConfig;
    std::string outputHeadFilePath;
    std::string outputSourceFilePath;

    std::vector<TagInfo> cacheTagsList;

    std::map<std::string, std::vector<TagInfo> > cacheTagsMap;
};


#endif //UNREALTAGAUTOCREATE_TAG_READ_PRINT_H
