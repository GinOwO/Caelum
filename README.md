# Caelum
[![LICENSE](https://img.shields.io/github/license/GinOwO/Caelum?color=blue)](LICENSE) ![Language](https://img.shields.io/badge/Language-C%2B%2B-brightgreen) ![Qt](https://img.shields.io/badge/Qt-6.5.0-brightgreen) ![GitHub csize](https://img.shields.io/github/languages/code-size/GinOwO/Caelum) [![Build Linux](https://github.com/GinOwO/Caelum/actions/workflows/c-cpp-build-linux.yaml/badge.svg?event=push)](https://github.com/GinOwO/Caelum/actions/workflows/c-cpp-build-linux.yaml) [![Build Windows](https://github.com/GinOwO/Caelum/actions/workflows/c-cpp-build-windows.yaml/badge.svg?event=push)](https://github.com/GinOwO/Caelum/actions/workflows/c-cpp-build-windows.yaml)

## Downloading

To use the pre-built binaries follow the steps below

### Linux

Download `Caelum_Linux_x86_64.tar.gz` from releases and extract the contents to a folder. Then run `Caelum-*.AppImage`

### Windows

Download `Caelum_Windows_x86_64.zip` from releases and extract the contents to a folder. Then run `Caelum.exe`

## Building From Source

### Prerequisites

To build Caelum, you require the following on your system:

- C++20 or above (MSVC will fail to build so go for GNU GCC)
- Qt framework 6.5.0+
- Qt Creator Community 10.0.0+
- (Optional: Linux only) [AppImage-Builder](https://appimage-builder.readthedocs.io/en/latest/intro/install.html)

### Cloning 

Open a command line in your desired folder, clone the repo with the following command
```
git clone https://github.com/GinOwO/Caelum.git
```

## Building

### Windows
1. Open Qt Creator and load the project.
2. Switch to release and then build the project.
3. Copy the release exe from the build folder to your desired folder.
4. Open a command line in your bin folder in the Qt installation, ex: `"D:\Qt\6.5.*\mingw_64\bin"`
5. Use the `set PATH=<path_to_qt>;%PATH%` to use the Qt DLLs, ex:`set PATH=D:\Qt\Tools\mingw1120_64\bin;%PATH%`
6. Now run the `windeployqt6 "<path_to_exe>"` command to copy necessary DLLs to the exe folder, ex: `windeployqt6 "D:\Caelum\Caelum.exe"`

### Linux
1. Open Qt Creator and load the project.
2. Switch to release and then build the project.
3. Copy the release build from the build folder to your desired folder. Make the directory `AppDir/usr/bin` and copy the release build to it.
4. Open a terminal in the folder and run `appimage-builder --generate && appimage-builder` to generate the AppImage.

## Usage
1. Launch Caelum.
2. Use the text editor to write your code or use the open file to open an existing file.
3. Click on the Build button or F6 to build the code.
4. Click on the Run button or F5 to run the code and see the output.

## Code Syntax
A limited set of x86_64 assembly instructions are supported. The supported instructions are listed below:
+------------------------------------------------------------------------+
| Instruction | Description                                              |
|-------------|----------------------------------------------------------|
| `MOV`       | Memory move. Implemented as a special case in `interpreter.cpp`.            |
| `XCHG`      | Exchange the values of two operands.                     |
| `LEA`       | Load effective address. Implemented as a special case in `interpreter.cpp`. |
| `INC`       | Increment the value of a register or memory location.    |
| `ADD`       | Add two operands and store the result.                   |
| `DEC`       | Decrement the value of a register or memory location.    |
| `SUB`       | Subtract two operands and store the result.              |
| `MUL`       | Multiply two operands and store the result.              |
| `DIV`       | Divide two operands and store the result.                |
| `AND`       | Bitwise AND operation.                                   |
| `ANDN`      | Bitwise AND-NOT operation.                               |
| `OR`        | Bitwise OR operation.                                    |
| `XOR`       | Bitwise XOR operation.                                   |
| `NOT`       | Bitwise NOT operation.                                   |
| `NOP`       | No operation.                                            |
| `HLT`       | Halt the CPU.                                            |
| `ADC`       | Add with carry.                                          |
| `SBB`       | Subtract with borrow.                                    |
| `AAA`       | ASCII adjust after addition.                             |
| `AAS`       | ASCII adjust after subtraction.                          |
| `AAM`       | ASCII adjust after multiplication.                       |
| `AAD`       | ASCII adjust before division.                            |
| `DAA`       | Decimal adjust after addition.                           |
| `DAS`       | Decimal adjust after subtraction.                        |
| `SHL`       | Shift left.                                              |
| `SHR`       | Shift right.                                             |
| `SAL`       | Arithmetic shift left.                                   |
| `SAR`       | Arithmetic shift right.                                  |
| `ROL`       | Rotate left through carry.                               |
| `ROR`       | Rotate right through carry.                              |
| `RCL`       | Rotate left through carry with double precision.         |
| `RCR`       | Rotate right through carry with double precision.        |
| `CMP`       | Compare two operands.                                    |
| `TEST`      | Logical compare.                                         |
| `JMP`       | Jump to a specified location.                            |
| `JE`        | Jump if equal (ZF=1).                                    |
| `JNE`       | Jump if not equal (ZF=0).                                |
| `JG`        | Jump if greater (SF=OF and ZF=0).                        |
| `JGE`       | Jump if greater or equal (SF=OF).                        |
| `JL`        | Jump if less (SF!=OF).                                   |
| `JLE`       | Jump if less or equal (ZF=1 or SF!=OF).                  |
| `JZ`        | Jump if zero (ZF=1).                                     |
| `JNZ`       | Jump if not zero (ZF=0).                                 |
| `JP`        | Jump if parity (PF=1).                                   |
| `JNP`       | Jump if not parity (PF=0).                               |
| `JPE`       | Jump if parity even (PF=1).                              |
| `JPO`       | Jump if parity odd (PF=0).                               |
| `JO`        | Jump if overflow (OF=1).                                 |
| `JNO`       | Jump if no overflow (OF=0).                              |
| `JC`        | Jump if carry (CF=1).                                    |
| `JNC`       | Jump if no carry (CF=0).                                 |
| `JS`        | Jump if sign (SF=1).                                     |
| `JNS`       | Jump if no sign (SF=0).                                  |
| `LOOP`      | Loop instruction.                                        |
| `LOOPE`     | Loop while equal (ZF=1).                                 |
| `LOOPNE`    | Loop while not equal (ZF=0).                             |
| `RET`       | Return from subroutine.                                  |
| `ADCX`      | Add with carry into the CF.                              |
| `SBBX`      | Subtract with borrow from the CF.                        |
| `SHLX`      | Shift left using the CL register.                        |
| `SHRX`      | Shift right using the CL register.                       |
| `SALX`      | Arithmetic shift left using the CL register.             |
| `SARX`      | Arithmetic shift right using the CL register.            |
| `RORX`      | Rotate right using the CL register.                      |
| `PUSH`      | Push data onto the stack.                                |
| `POP`       | Pop data from the stack.                                 |
+------------------------------------------------------------------------+

### Non-Standard Instructions
These are instructions that are not part of the x86_64 instruction set but are implemented in Caelum using higher level C/C++ constructs.
+------------------------------------------------------------------------+
| Instruction | Description                                              |
|-------------|----------------------------------------------------------|
| `PUTS`      | Print a number to the console.                           |
| `FLUSH`     | Flush the out buffer.                                    |
+------------------------------------------------------------------------+

## Notes
- All programs must start with `global <label>` and end with `hlt`.
- All labels must be unique.
- The entry point of the program is the label specified after global.
- All instructions are case insensitive.

## Contributing
Please read [CONTRIBUTING.md](./.github/CONTRIBUTING.md) for details on contributing to this project.

## License

This project is licensed under [The Unlicense](LICENSE).
