#include <QtCore>

static const QStringList sourceExtensions = { "*.cc", "*.h", "*.hpp", "*.cpp" };
static const QStringList ignoredFileNameStartsWith = { "moc_", "qrc_", "ui_" };

#include "helpers.h"

bool isIgnoredFile(const QString& fileName)
{
    for (auto ext : ignoredFileNameStartsWith)
        if (fileName.startsWith(ext, Qt::CaseInsensitive))
            return true;

    return false;
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

    // Positional argument for path (optional)
    parser.addPositionalArgument("path", "The path to process. Default is current directory.");

    // Process the actual command line arguments given by the user
    parser.process(app);

    // Get the optional path argument or use current path
    QString targetPath = parser.positionalArguments().isEmpty() ? QDir::currentPath() : parser.positionalArguments().first();
    QStringList ignoredFiles;

    // Get the skip option if provided
    if (parser.isSet(skipOption)) {
        QString skipValue = parser.value(skipOption);
        ignoredFiles = skipValue.split(',', Qt::SkipEmptyParts);
    }

    qInfo() << "Dir: " << QDir::toNativeSeparators(targetPath);

    if (!ignoredFiles.isEmpty()) 
    {
        qInfo() << "Ignored file names: " << ignoredFiles;
    }
    else {
        qInfo() << "No files to skip.";
    }

    QDir dir(targetPath);

    QDirIterator it(targetPath, sourceExtensions, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    qint64 totalSizeInBytes = 0;
    int nTotalFiles = 0;

    int codeLines = 0;

    while (it.hasNext())
    {
        QString path = it.next();

        QString fullPath = it.filePath();

        QFileInfo fi = it.fileInfo();

        if (isIgnoredFile(fi.fileName()))
            continue;

        if (ignoredFiles.contains(fi.fileName(), Qt::CaseInsensitive))
            continue;

        QString dirName = QFileInfo(fi.path()).fileName();

        if (ignoredFiles.contains(dirName, Qt::CaseInsensitive))
            continue;

        //qInfo() << "File: " << fullPath;

        totalSizeInBytes += fi.size();
        codeLines += Helpers::countCodeLines(fullPath);

        nTotalFiles++;
    }

    qInfo() << "Total code files: " << nTotalFiles;
    qInfo() << "Code size: " << Helpers::formatFileSize(totalSizeInBytes);
    qInfo() << "Code lines: " << codeLines;

    return 0; // app.exec();
}
