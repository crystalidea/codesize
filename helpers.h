#pragma once

#include <QtCore>

namespace Helpers
{
    QString formatFileSize(qint32 size);
    QString formatCodeLines(qint32 lines);
    uint32_t countCodeLines(const QString& filePath);
}

// add QString to std::formatCodeLines

template <>
struct std::formatter<QString, char> {
    // Parsing method
    constexpr auto parse(std::format_parse_context& ctx) {
        // Parse the format specifier if any
        auto it = ctx.begin();
        auto end = ctx.end();

        if (it != end && *it != '}') {
            throw std::format_error("Invalid format specifier");
        }
        return it;
    }

    // Formatting method
    auto format(const QString& qstr, std::format_context& ctx) const {
        // Convert QString to std::string and format it
        auto str = qstr.toStdString();
        return std::format_to(ctx.out(), "{}", str);
    }
};