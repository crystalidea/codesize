#include <QtCore>

#include <format>
#include <iostream>
#include <vector>
#include <string>

#include "helpers.h"

class CodeType
{
public:

    virtual QString getName() const = 0;
    virtual QStringList getExtensions() const = 0;
    virtual bool isIgnoredFile(const QString &fileName) const = 0;
};

class CodeTypeCpp : public CodeType
{
public:

    QString getName() const
    {
        return "C/C++";
    }

    QStringList getExtensions() const override
    {
        return { "*.cc", "*.h", "*.hpp", "*.cpp", "*.c", "*.m", "*.mm" };
    }

    bool isIgnoredFile(const QString& fileName) const override
    {
        static const QStringList ignoredFileNameStartsWith = { "moc_", "qrc_", "ui_" };

        for (auto ext : ignoredFileNameStartsWith)
            if (fileName.startsWith(ext, Qt::CaseInsensitive))
                return true;

        return false;
    }
};

class UserIgnoredFiles
{
public:

    void addFiles(const QStringList& files)
    {
        _ignoredFiles = files;
    }

    bool matchesFilePath(const QString& fullPath) const
    {
        QFileInfo fi(fullPath);

        for (const QString& skipped : _ignoredFiles)
        {
            QString dirName = QChar('/') + skipped + QChar('/');

            if (fullPath.contains(dirName) || skipped.compare(fi.fileName(), Qt::CaseInsensitive) == 0)
            {
                return true;
            }
        }

        return false;
    }

private:

    QStringList _ignoredFiles;
};

using codeSizeResult = std::vector<uint32_t>; // bytes, code lines, total files

codeSizeResult processCodeDirectory(const QString& targetPath, const CodeType *codeType, UserIgnoredFiles *pUserIgnored, bool verbose)
{
    uint32_t totalSizeInBytes = 0;
    uint32_t nTotalFiles = 0;
    uint32_t codeLines = 0;

    QDir dir(targetPath);

    QDirIterator it(targetPath, codeType->getExtensions(), QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        QString path = it.next();
        QString fullPath = it.filePath();
        QFileInfo fi = it.fileInfo();

        bool bSkip = false;

        if (codeType->isIgnoredFile(fi.fileName()))
        {
            if (verbose)
                qInfo().noquote() << "Ignored file: " << QDir::toNativeSeparators(fullPath);

            bSkip = true;
        }
        else if (pUserIgnored->matchesFilePath(fullPath))
        {
            if (verbose)
                qInfo().noquote() << "User ignored file: " << QDir::toNativeSeparators(fullPath);

            bSkip = true;
        }
        
        if (bSkip)
            continue;
        
        if (verbose)
            qInfo().noquote() << "Code file: " << QDir::toNativeSeparators(fullPath);

        totalSizeInBytes += fi.size();
        codeLines += Helpers::countCodeLines(fullPath);

        nTotalFiles++;
    }

    return { totalSizeInBytes, codeLines, nTotalFiles };
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;

    parser.setApplicationDescription("codesize");
    parser.addHelpOption();

    // Add the /skip option
    QCommandLineOption skipOption(QStringList() << "skip", "Comma-separated list of files to skip.", "ignoredFiles");
    parser.addOption(skipOption);

    // Add the --verbose switch
    QCommandLineOption verboseOption(QStringList() << "verbose", "Enable verbose output.");
    parser.addOption(verboseOption);

    // Positional argument for path (optional)
    parser.addPositionalArgument("path", "The path to process. Default is current directory.");

    // Process the actual command line arguments given by the user
    parser.process(app);

    // Check if verbose mode is enabled
    bool verbose = parser.isSet(verboseOption);

    QStringList targetPaths;
    
    if (parser.positionalArguments().size())
    {
        for (qsizetype i = 0; i < parser.positionalArguments().size(); i++)
        {
            targetPaths.push_back(parser.positionalArguments().at(i));

            if (verbose)
                qInfo().noquote() << "Dir: " << QDir::toNativeSeparators(targetPaths.last());
        }
    }
    else
    {
        targetPaths = { QDir::currentPath() };

        if (verbose)
            qInfo().noquote() << "Dir: " << QDir::toNativeSeparators(targetPaths.first());
    }

    CodeType* pCodeType = nullptr;

    UserIgnoredFiles userIgnored;

    // Get the skip option if provided
    if (parser.isSet(skipOption)) 
    {
        QString skipValue = parser.value(skipOption);
        QStringList fs = skipValue.split(',', Qt::SkipEmptyParts);

        userIgnored.addFiles(fs);
    }

    pCodeType = new CodeTypeCpp(); // TODO: add others

    uint32_t totalSizeInBytes = 0;
    uint32_t nTotalFiles = 0;
    uint32_t codeLines = 0;

    for (const QString& targetPath : targetPaths)
    {
        codeSizeResult r = processCodeDirectory(targetPath, pCodeType, &userIgnored, verbose);

        totalSizeInBytes += r[0];
        codeLines += r[1];
        nTotalFiles += r[2];
    }

    std::cout << std::format("{} code size: {}\n", pCodeType->getName(), Helpers::formatFileSize(totalSizeInBytes));
    std::cout << std::format("{} code lines: {}\n", pCodeType->getName(), Helpers::formatCodeLines(codeLines));
    std::cout << std::format("{} code files: {}\n", pCodeType->getName(), nTotalFiles);

    delete pCodeType;

    return 0; // app.exec();
}
