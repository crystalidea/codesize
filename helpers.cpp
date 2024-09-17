#include "pch.h"
#include "helpers.h"

#include <fstream>
#include <iostream>
#include <string_view>
#include <ranges>
#include <sstream>
#include <iomanip>
#include <algorithm>

std::string Helpers::formatFileSize(uint32_t size)
{
    const int64_t KB = 1024;
    const int64_t MB = 1024 * KB;
    const int64_t GB = 1024 * MB;
    const int64_t TB = 1024 * GB;

    std::ostringstream oss;

    if (size >= TB)
    {
        oss << std::fixed << std::setprecision(2) << (size / static_cast<double>(TB)) << " TB";
    }
    else if (size >= GB)
    {
        oss << std::fixed << std::setprecision(2) << (size / static_cast<double>(GB)) << " GB";
    }
    else if (size >= MB)
    {
        oss << std::fixed << std::setprecision(2) << (size / static_cast<double>(MB)) << " MB";
    }
    else if (size >= KB)
    {
        oss << std::fixed << std::setprecision(2) << (size / static_cast<double>(KB)) << " KB";
    }
    else
    {
        oss << size << " bytes";
    }

    return oss.str();
}

struct file_size_facet : public std::numpunct<char> {
    explicit file_size_facet(size_t refs = 0) : std::numpunct<char>(refs) {}
    virtual char do_thousands_sep() const { return '.'; }
    virtual std::string do_grouping() const { return "\003"; }
};

std::string Helpers::formatCodeLines(uint32_t lines)
{
    static std::locale withgroupings(std::locale(), new file_size_facet);

    return std::format(withgroupings, "{:L}", lines);
}

uint32_t Helpers::countCodeLines(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (file.is_open())
    {
        uint32_t lineCount = 0;
        std::string line;

        while (std::getline(file, line))
        {
            // Trim leading and trailing whitespace
            line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) { return !std::isspace(ch); }));
            line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), line.end());

            if (line == "}" || line == "{") // we're not interested in these lines
                continue;
            
            if (!line.empty())
            {
                ++lineCount;
            }
        }

        file.close();
        return lineCount;
    }

    std::cerr << "Cannot open file: " << filePath << std::endl;
    return static_cast<uint32_t>(-1); // or return 0 if you prefer
}

// Helper function to convert a string to lowercase using C++20 features
std::string toLowerCase(const std::string& str) {
    std::string result;
    std::ranges::transform(str, std::back_inserter(result), [](unsigned char c) {
        return std::tolower(c);
        });
    return result;
}

bool Helpers::startsWith(const std::string& mainStr, const std::string& toMatch)
{
    if (mainStr.size() < toMatch.size())
        return false;

    // Create views for the beginning of mainStr and the entire toMatch
    auto mainStrView = mainStr | std::ranges::views::take(toMatch.size());
    auto toMatchView = toMatch;

    // Use std::ranges::equal with a case-insensitive comparator
    return std::ranges::equal(mainStrView, toMatchView,
        [](char a, char b) {
            return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
        });
}

bool Helpers::compareNoCase(const std::string& str1, const std::string& str2)
{
    return toLowerCase(str1) == toLowerCase(str2);
}

bool Helpers::containsNoCase(const std::string& str1, const std::string& str2)
{
    std::string lowerStr1 = toLowerCase(str1);
    std::string lowerStr2 = toLowerCase(str2);

    // Check if lowerStr1 contains lowerStr2
    return lowerStr1.find(lowerStr2) != std::string::npos;
}
