#include "pch.h"
#include "code_type.h"
#include "user_ignored_files.h"
#include "helpers.h"

bool CodeType::isMyFile(const fs::path& file) const
{
    string extension = file.extension().string();

    return std::find(_extensions.begin(), _extensions.end(), extension) != _extensions.end();
}

CodeSizeResult CodeType::processDirectory(const string& directoryPath, const UserIgnoredFiles* pUserIgnored, bool verbose) const
{
    uint32_t totalSizeInBytes = 0;
    uint32_t nTotalFiles = 0;
    uint32_t codeLines = 0;

    if (fs::exists(directoryPath) && fs::is_directory(directoryPath))
    {
        for (const auto& entry : fs::recursive_directory_iterator(directoryPath))
        {
            std::string filename = entry.path().filename().string();
            std::string fullPath = entry.path().string();

            if (fs::is_regular_file(entry.status()))
            {
                // we can safely assume the code file size is less than 4GB
                uint32_t fileSize = static_cast<uint32_t>(fs::file_size(entry.path()));

                if (isMyFile(entry.path())) // Check if the extension matches any in the filter list
                {
                    bool bSkip = false;

                    if (isIgnoredFile(filename))
                    {
                        if (verbose)
                            std::cout << "Ignored file: " << fullPath << std::endl;

                        bSkip = true;
                    }
                    else if (pUserIgnored->matchesFile(fullPath))
                    {
                        if (verbose)
                            std::cout << "User ignored file: " << fullPath << std::endl;

                        bSkip = true;
                    }

                    if (bSkip)
                        continue;

                    if (verbose)
                        std::cout << "Code file: " << fullPath << std::endl;

                    totalSizeInBytes += fileSize;
                    codeLines += Helpers::countCodeLines(fullPath);

                    nTotalFiles++;
                }
            }
        }
    }
    else
    {
        std::cerr << "The provided path is not a directory or does not exist.\n";
    }

    return CodeSizeResult(totalSizeInBytes, nTotalFiles, codeLines);
}
