### codesize

Simple command line tool that calculates the total code (currently C/C++ only) size of a directory including code lines count. Can be compiled with any C++ 20 compiler.

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
