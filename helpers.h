#pragma once

#include <string>

namespace Helpers
{
    std::string formatFileSize(uint32_t size);
    std::string formatCodeLines(uint32_t lines);
    uint32_t countCodeLines(const std::string& filePath);

    // Function to check if a string starts with another string (case insensitive)
    bool startsWith(const std::string& mainStr, const std::string& toMatch);
    bool compareNoCase(const std::string& str1, const std::string& str2);

    bool containsNoCase(const std::string& str1, const std::string& str2);
}