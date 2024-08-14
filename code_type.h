#pragma once

#include "code_size_result.h"

class UserIgnoredFiles;

class CodeType
{
public:

    virtual string getName() const = 0;
    virtual bool isIgnoredFile(const string&fileName) const = 0;

    bool isMyFile(const fs::path& file) const;

    CodeSizeResult processDirectory(const string& directoryPath, const UserIgnoredFiles* pUserIgnored, bool verbose) const;

 protected:

    string_list _extensions;
};