#pragma once

class UserIgnoredFiles
{
public:

    UserIgnoredFiles() = default;
    UserIgnoredFiles(const string& cmdOption);

    bool matchesFile(const fs::path& file_path) const;

private:

    string_list _ignoredFiles;
};