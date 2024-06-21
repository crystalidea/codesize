#include <QtCore/QCoreApplication>

#include <QtCore>

QString formatFileSize(qint64 size) {
    const qint64 KB = 1024;
    const qint64 MB = 1024 * KB;
    const qint64 GB = 1024 * MB;
    const qint64 TB = 1024 * GB;

    if (size >= TB)
        return QString::number(size / (double)TB, 'f', 2) + " TB";
    else if (size >= GB)
        return QString::number(size / (double)GB, 'f', 2) + " GB";
    else if (size >= MB)
        return QString::number(size / (double)MB, 'f', 2) + " MB";
    else if (size >= KB)
        return QString::number(size / (double)KB, 'f', 2) + " KB";
    else
        return QString::number(size) + " bytes";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString targetPath = QDir::currentPath();
    
    if (a.arguments().size() > 1)
        targetPath = a.arguments().at(1);

    qInfo() << "Dir: " << QDir::toNativeSeparators(targetPath);

    QDir dir(targetPath);

    QDirIterator it(targetPath, { "*.cc", "*.h", "*.cpp" }, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    qint64 totalSizeInBytes = 0;
    int nTotalFiles = 0;

    while (it.hasNext())
    {
        QString path = it.next();

        QString fullPath = it.filePath();

        if (fullPath.contains("moc_") || fullPath.contains("qrc_") || fullPath.contains("ui_"))
            continue;

        //qInfo() << "File: " << fullPath;

        totalSizeInBytes += it.fileInfo().size();

        nTotalFiles++;
    }

    qInfo() << "Total code files: " << nTotalFiles;
    qInfo() << "Code size: " << formatFileSize(totalSizeInBytes);

    return 0; // a.exec();
}
