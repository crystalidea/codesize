#pragma once

#include <QtCore>

namespace Helpers
{
    QString formatFileSize(qint64 size);
    int countCodeLines(const QString& filePath);
}