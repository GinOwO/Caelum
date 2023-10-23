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

## Instruction Set
A limited set of x86_64 assembly instructions are supported. The supported instructions can be found at the wiki [here](https://github.com/GinOwO/Caelum/wiki)

## Notes
- All programs must start with `global <label>` and end with `hlt`.
- All labels must be unique.
- The entry point of the program is the label specified after global.
- All instructions are case insensitive.
- Special Registers like SP currently do nothing.

## Contributing
Please read [CONTRIBUTING.md](./.github/CONTRIBUTING.md) for details on contributing to this project.

## License

This project is licensed under [The Unlicense](LICENSE).
