#include "pch.h"
#include "helpers.h"
#include "code_size_result.h"
#include "code_type.h"
#include "code_type_cpp.h"
#include "user_ignored_files.h"

#include <CLI11.hpp> // 3rdParty

int main(int argc, char *argv[])
{
    CLI::App cliApp{ "codesize" };

    string_list targetPaths;
    cliApp.add_option("paths", targetPaths, "The paths to process. Default is current directory if none are provided.");

    // Add the --skip option
    std::string userSkip;
    cliApp.add_option("--skip", userSkip, "Comma-separated list of files to skip.");

    // Add the --verbose switch
    bool verbose = false;
    cliApp.add_flag("--verbose", verbose, "Enable verbose output.");

    // Parse command-line arguments
    CLI11_PARSE(cliApp, argc, argv);

    // If no paths provided, default to the current directory
    if (targetPaths.empty())
        targetPaths = { std::filesystem::current_path().string() };
    
    CodeType* pCodeType = nullptr;

    UserIgnoredFiles userIgnored(userSkip);

    pCodeType = new CodeTypeCpp(); // TODO: add others, choose from command line

    CodeSizeResult resultTotal;

    for (const auto & path : targetPaths)
    {
        resultTotal += pCodeType->processDirectory(path, &userIgnored, verbose);
    }

    resultTotal.print(pCodeType->getName());

    delete pCodeType;

    return 0;
}
