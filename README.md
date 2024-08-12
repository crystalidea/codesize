### codesize

Simple Qt-based command line tool that calculates the total C++ code size of a directory including code lines count. There's only MSVC solution currently but the app is not limited to Windows or any compiler. Can be compiled against Qt5/Qt6. A pull request with CMake build configuration is welcome here.

**Example usage**:

```bash
./codesize ~/projects/mySourceDir
```

```
 C++ code size: 33.40 MB
C++ code lines: 893.932
C++ code files: 2076
```

You can pass arbitrary number of directories.
Optionally you can pass --skip switch to specify comma-separated list of file/folder names to skip.
