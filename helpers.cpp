#include "helpers.h"

#include <format>
#include <string>
#include <string_view>

QString Helpers::formatFileSize(qint32 size)
{
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

struct file_size_facet : public std::numpunct<char> {
    explicit file_size_facet(size_t refs = 0) : std::numpunct<char>(refs) {}
    virtual char do_thousands_sep() const { return '.'; }
    virtual std::string do_grouping() const { return "\003"; }
};

QString Helpers::formatCodeLines(qint32 lines)
{
    static std::locale withgroupings(std::locale(), new file_size_facet);

    std::string size_formatted = std::format(withgroupings, "{:L}", lines);

    return QString::fromStdString(size_formatted);
}

int Helpers::countCodeLines(const QString& filePath) 
{
    QFile file(filePath);
    
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) 
    {
        QTextStream in(&file);
        int lineCount = 0;
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (!line.trimmed().isEmpty()) {
                ++lineCount;
            }
        }

        file.close();

        return lineCount;
    }
    
    qWarning() << "Cannot open file:" << filePath;

    return -1;
}