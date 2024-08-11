### codesize

Simple Qt-based command line tool that calculates the total C++ code size of a directory including code lines count. There's only MSVC solution currently but the app is not limited to Windows or any compiler. Can be compiled against Qt5/Qt6. A pull request with CMake build configuration is welcome here.

**Example usage**:

```bash
./codesize ~/projects/mySourceDir
```

```
Dir:  "~/projects/mySourceDir"
Total code files:  2076
Code size:  "33.40 MB"
Code lines:  893932
```
