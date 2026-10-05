# Preprocessor Mini Project in C

## Objective
This student-level project creates a program named `mypreprocessor`. It reads a C source file and writes a processed file with the `.i` extension.

## Features
- Removes `//` single-line comments and `/* ... */` multi-line comments.
- Processes local `#include` files by copying their contents into the output.
- Supports simple macros such as `#define PI 3.14`.
- Supports one-argument macros such as `#define SQUARE(x) ((x)*(x))`.
- Uses separate `.c` and `.h` files.
- Uses a Makefile.

## Files
- `main.c`: command-line entry point
- `preprocessor.c`: coordinates processing
- `comment_removal.c/.h`: removes comments
- `macro_handler.c/.h`: stores and substitutes macros
- `file_inclusion.c/.h`: handles include lines
- `Makefile`: compiles the project
- `inputfile.c`: sample input

## Build
Run this in a terminal with GCC and Make installed:

```sh
make
```

## Run
```sh
./mypreprocessor inputfile.c
```
This creates `inputfile.i`.

On Windows with MinGW, run `.\mypreprocessor.exe inputfile.c`.

## Limitations
This is a learning project and not a complete replacement for the real C preprocessor. The include handler copies local files and does not recursively preprocess the included file. It supports common examples from the assignment rather than every rule of the C language.
