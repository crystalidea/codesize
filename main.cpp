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
    QCoreApplication a(argc, argv);

    QString targetPath = QDir::currentPath();
    
    if (a.arguments().size() > 1)
        targetPath = a.arguments().at(1);

    qInfo() << "Dir: " << QDir::toNativeSeparators(targetPath);

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

        //qInfo() << "File: " << fullPath;

        totalSizeInBytes += fi.size();
        codeLines += Helpers::countCodeLines(fullPath);

        nTotalFiles++;
    }

    qInfo() << "Total code files: " << nTotalFiles;
    qInfo() << "Code size: " << Helpers::formatFileSize(totalSizeInBytes);
    qInfo() << "Code lines: " << codeLines;

    return 0; // a.exec();
}
