# Caelum
[![LICENSE](https://img.shields.io/github/license/GinOwO/Caelum?color=blue)](LICENSE) ![Language](https://img.shields.io/badge/Language-C%2B%2B-brightgreen) ![Qt](https://img.shields.io/badge/Qt-6.5.0-brightgreen) ![GitHub csize](https://img.shields.io/github/languages/code-size/GinOwO/Caelum) [![Build Linux](https://github.com/GinOwO/Caelum/actions/workflows/c-cpp-build-linux.yaml/badge.svg?event=push)](https://github.com/GinOwO/Caelum/actions/workflows/c-cpp-build-linux.yaml) [![Build Windows](https://github.com/GinOwO/Caelum/actions/workflows/c-cpp-build-windows.yaml/badge.svg?event=push)](https://github.com/GinOwO/Caelum/actions/workflows/c-cpp-build-windows.yaml)

WIP

## Downloading

To use the pre-built binaries follow the steps below

### Linux

Download `Caelum_Linux_x86_64.tar.gz` from releases and extract the contents to a folder. Then run `/DistributionKit/Caelum.sh`

### Windows

Download `Caelum_Windows_x86_64.zip` from releases and extract the contents to a folder. Then run `Caelum.exe`

## Building From Source

### Prerequisites

To build Caelum, you require the following on your system:

- C++20 or above (MSVC will fail to build so go for GNU GCC)
- Qt framework 6.5.0+
- Qt Creator Community 10.0.0+
- (Optional: Linux Building only) [CQtDeployer](https://github.com/QuasarApp/CQtDeployer)

### Cloning 

Open a command line in your desired folder, clone the repo with the following command
```
git clone https://github.com/KThankYou/Caelum.git
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
3. Copy the release build from the build folder to your desired folder.
4. Open a terminal in the folder and copy the necessary files with CQtDeployer using `cqtdeployer -bin Caelum -qmake /path/to/Qt/6.5.*/gcc_64/bin/qmake`

## Usage
WIP

## Notes
WIP

## Contributing
Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on contributing to this project.

## License

This project is licensed under [The Unlicense](LICENSE).
