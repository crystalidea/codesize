#pragma once

class CodeSizeResult
{
public:

    CodeSizeResult() = default;

    CodeSizeResult(uint32_t sizeInBytes, uint32_t totalFiles, uint32_t codeLines)
        : _nSizeInBytes(sizeInBytes), _nTotalFiles(totalFiles), _codeLines(codeLines) {}

    CodeSizeResult operator+(const CodeSizeResult& other) const {
        return CodeSizeResult(
            this->_nSizeInBytes + other._nSizeInBytes,
            this->_nTotalFiles + other._nTotalFiles,
            this->_codeLines + other._codeLines
        );
    }

    CodeSizeResult& operator+=(const CodeSizeResult& other) {
        this->_nSizeInBytes += other._nSizeInBytes;
        this->_nTotalFiles += other._nTotalFiles;
        this->_codeLines += other._codeLines;
        return *this;
    }

    void print(const string &codeTypeName) const;

private:

    uint32_t _nSizeInBytes = 0;
    uint32_t _nTotalFiles = 0;
    uint32_t _codeLines = 0;
};