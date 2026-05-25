#include "pch.h"
#include "code_type.h"
#include "user_ignored_files.h"
#include "helpers.h"

#include <algorithm>

bool CodeType::isMyFile(const fs::path& file) const
{
    string extension = file.extension().string();

    // Match extensions case-insensitively (e.g. .CPP, .H on Windows)
    return std::any_of(_extensions.begin(), _extensions.end(),
        [&extension](const string& ext) { return Helpers::compareNoCase(ext, extension); });
}

CodeSizeResult CodeType::processDirectory(const string& directoryPath, const UserIgnoredFiles* pUserIgnored, bool verbose) const
{
    uint32_t totalSizeInBytes = 0;
    uint32_t nTotalFiles = 0;
    uint32_t codeLines = 0;

    if (fs::exists(directoryPath) && fs::is_directory(directoryPath))
    {
        std::error_code ec;
        fs::recursive_directory_iterator iter(
            directoryPath, fs::directory_options::skip_permission_denied, ec);

        if (ec)
        {
            std::cerr << "Cannot read directory: " << directoryPath
                      << " (" << ec.message() << ")\n";
            return CodeSizeResult(totalSizeInBytes, nTotalFiles, codeLines);
        }

        const fs::recursive_directory_iterator end;

        while (iter != end)
        {
            const fs::directory_entry& entry = *iter;

            std::error_code fileEc;
            if (entry.is_regular_file(fileEc) && !fileEc)
            {
                std::string filename = entry.path().filename().string();
                std::string fullPath = entry.path().string();

                if (isMyFile(entry.path())) // Check if the extension matches any in the filter list
                {
                    // we can safely assume the code file size is less than 4GB
                    uint32_t fileSize = static_cast<uint32_t>(fs::file_size(entry.path(), fileEc));

                    if (!fileEc)
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

                        if (!bSkip)
                        {
                            if (verbose)
                                std::cout << "Code file: " << fullPath << std::endl;

                            totalSizeInBytes += fileSize;
                            codeLines += Helpers::countCodeLines(fullPath);

                            nTotalFiles++;
                        }
                    }
                }
            }

            iter.increment(ec);

            if (ec)
            {
                std::cerr << "Stopping directory scan: " << ec.message() << "\n";
                break;
            }
        }
    }
    else
    {
        std::cerr << "The provided path is not a directory or does not exist.\n";
    }

    return CodeSizeResult(totalSizeInBytes, nTotalFiles, codeLines);
}
