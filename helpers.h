#pragma once

#include <QtCore>

namespace Helpers
{
    QString formatFileSize(qint32 size);
    QString formatCodeLines(qint32 lines);
    int countCodeLines(const QString& filePath);
}