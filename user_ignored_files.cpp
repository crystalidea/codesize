#include "pch.h"
#include "user_ignored_files.h"
#include "helpers.h"

UserIgnoredFiles::UserIgnoredFiles(const string& cmdOption)
{
    if (!cmdOption.empty()) {
        std::stringstream ss(cmdOption);
        std::string item;
        while (std::getline(ss, item, ',')) {
            _ignoredFiles.push_back(item);
        }
    }
}

bool UserIgnoredFiles::matchesFile(const fs::path& file_path) const
{
    const static string separator(1, fs::path::preferred_separator);

    std::string fullPath = file_path.string();
    std::string filename = file_path.filename().string();

    for (const string& skipped : _ignoredFiles)
    {
        std::string dir_name = separator + skipped + separator;

        if (Helpers::compareNoCase(filename, skipped) ||
            Helpers::containsNoCase(fullPath, dir_name))
            return true;
    }

    return false;
}