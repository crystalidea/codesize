#include <QtCore>

static const QStringList sourceExtensions = { "*.cc", "*.h", "*.hpp", "*.cpp", "*.c", "*.m", "*.mm"};
static const QStringList ignoredFileNameStartsWith = { "moc_", "qrc_", "ui_" };

#include "helpers.h"

bool isIgnoredFile(const QString& fileName)
{
    for (auto ext : ignoredFileNameStartsWith)
        if (fileName.startsWith(ext, Qt::CaseInsensitive))
            return true;

    return false;
}

using codeSizeResult = QVector<quint32>; // bytes, code lines, total files

codeSizeResult processCodeDirectory(const QString& targetPath, const QStringList ignoredFiles, bool verbose)
{
    quint32 totalSizeInBytes = 0;
    quint32 nTotalFiles = 0;
    quint32 codeLines = 0;

    QDir dir(targetPath);

    QDirIterator it(targetPath, sourceExtensions, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        QString path = it.next();
        QString fullPath = it.filePath();
        QFileInfo fi = it.fileInfo();

        bool bSkip = false;

        if (isIgnoredFile(fi.fileName()))
        {
            if (verbose)
                qInfo().noquote() << "Ignored file: " << QDir::toNativeSeparators(fullPath);

            bSkip = true;
        }
        else
        {
            for (const QString& skipped : ignoredFiles)
            {
                QString dirName = QChar('/') + skipped + QChar('/');

                if (fullPath.contains(dirName) || skipped.compare(fi.fileName(), Qt::CaseInsensitive) == 0)
                {
                    if (verbose)
                        qInfo().noquote() << "Skipped file: " << QDir::toNativeSeparators(fullPath);

                    bSkip = true;

                    break;
                }
            }
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
    QStringList ignoredFiles;

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

    // Get the skip option if provided
    if (parser.isSet(skipOption)) 
    {
        QString skipValue = parser.value(skipOption);
        ignoredFiles = skipValue.split(',', Qt::SkipEmptyParts);
    }

    quint32 totalSizeInBytes = 0;
    quint32 nTotalFiles = 0;
    quint32 codeLines = 0;

    for (const QString& targetPath : targetPaths)
    {
        codeSizeResult r = processCodeDirectory(targetPath, ignoredFiles, verbose);

        totalSizeInBytes += r[0];
        codeLines += r[1];
        nTotalFiles += r[2];
    }

    qInfo().noquote() << " C++ code size: " << Helpers::formatFileSize(totalSizeInBytes);
    qInfo().noquote() << "C++ code lines: " << Helpers::formatCodeLines(codeLines);
    qInfo().noquote() << "C++ code files: " << nTotalFiles;

    return 0; // app.exec();
}
