#pragma once

class CodeTypeCpp : public CodeType
{
public:

    CodeTypeCpp()
    {
        _extensions = { ".cc", ".h", ".hpp", ".cpp", ".c", ".m", ".mm" };
    }

    std::string getName() const override
    {
        return "C/C++";
    }

    bool isIgnoredFile(const string& fileName) const override
    {
        static const string_list ignoredFileNameStartsWith = { "moc_", "qrc_", "ui_" }; # Qt generated

        for (auto ext : ignoredFileNameStartsWith)
            if (Helpers::startsWith(fileName, ext))
                return true;

        return false;
    }
};