#include "pch.h"
#include "code_size_result.h"
#include "helpers.h"

void CodeSizeResult::print(const string& codeTypeName) const
{
    std::cout << std::format("{} code size: {}\n", codeTypeName, Helpers::formatFileSize(_nSizeInBytes));
    std::cout << std::format("{} code lines: {}\n", codeTypeName, Helpers::formatCodeLines(_codeLines));
    std::cout << std::format("{} code files: {}\n", codeTypeName, _nTotalFiles);
}
