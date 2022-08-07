# NAppGUI
Cross-Platform C SDK. Build portable desktop applications for Windows, macOS and Linux, using just C.

![NAppGUI Stack](https://nappgui.com/img/start/nappgui_stack.png)

## Quick start using the BUSY build system

NAppGUI and it's utility and demo applications can be built ab initio (i.e. with no system requirements 
beyond those of NAppGUI, particularly no Cmake) using the BUSY build system. Both static and shared 
library versions of NAppGUI are supported.

Proceed as follows:

1) Open a terminal window which is correctly set up for the intended toolchain; i.e. Visual Studio on Windows
comes with pre-declared macros, such as vcvars32.bat or vcvars64.bat, see https://docs.microsoft.com/en-us/cpp/build/building-on-the-command-line?view=msvc-170; on mac and linux gcc is usually directly available in the terminal without additional steps.
2) git clone --depth 1 https://github.com/frang75/nappgui_src.git nappgui_sdk
3) Create a build subdirectory in the nappgui_sdk directory, e.g. build
4) Download and unpack the BUSY source files to the build directory; you can use this link: https://github.com/rochus-keller/BUSY/archive/refs/heads/main.zip
5) Set the current directory to the build directory.
Then...

...with GCC & CLANG on Linux/Unix:
6) gcc *.c -O2 -lm -O2 -o lua
7) ./lua build.lua

...with GCC & CLANG on Windows:
6) gcc *.c -O2 -lm -O2 -o lua.exe
7) lua build.lua

...with MSVC on Windows:
6) cl /O2 /MD /Fe:lua.exe *.c
7) lua build.lua

8) wait until the build finishes (half a minute); the built libraries are in the output/src subdirectory

## Quick start in Windows

### Prerequisites
- [Visual Studio](https://visualstudio.microsoft.com/vs/)
- [CMake](https://cmake.org/download/)

### Open the Developer Command Prompt
```
(from C:\)

// Clone the NAppGUI repo
git clone --depth 1 https://github.com/frang75/nappgui_src.git nappgui_sdk

// Create a build directory
mkdir nappgui_build
cd nappgui_build

// Generate the Visual Studio Solution
cmake -G "Visual Studio 16 2019" ../nappgui_sdk/src

// Build the examples
msbuild NAppGUI.sln

// Run examples in 'demo' and 'howto' folders
.\demo\die\Debug\Die.exe
.\demo\bricks\Debug\Bricks.exe
.\demo\products\Debug\Products.exe
.\howto\col2dhello\Debug\Col2dHello.exe
.\howto\guihello\Debug\GuiHello.exe
...
```
![Running demo project in Windows](https://nappgui.com/img/start/run_demo_windows.png)

## Quick start in macOS

### Prerequisites
- [Xcode](https://developer.apple.com/xcode/)
- [CMake](https://cmake.org/download/)

### Open the Terminal
```
(from your home dir ~/)

// Clone the NAppGUI repo
git clone --depth 1 https://github.com/frang75/nappgui_src.git nappgui_sdk

// Create a build directory
mkdir nappgui_build
cd nappgui_build

// Generate the Xcode project
cmake -G "Xcode" ../nappgui_sdk/src

// Build the examples
xcodebuild

// Run examples in 'demo' and 'howto' folders
./demo/die/Debug/Die.app/Contents/MacOS/Die
./demo/bricks/Debug/Bricks.app/Contents/MacOS/Bricks
./demo/products/Debug/Products.app/Contents/MacOS/Products
./howto/col2dhello/Debug/Col2dHello.app/Contents/MacOS/Col2dHello
./howto/guihello/Debug/GuiHello.app/Contents/MacOS/GuiHello
...
```
![Running demo project in macOS](https://nappgui.com/img/start/run_demo_macos.png)

## Quick start in Linux

### Prerequisites
```
// Development tools
sudo apt-get install gcc
sudo apt-get install g++
sudo apt-get install git
sudo apt-get install make
sudo apt-get install cmake

// Development libraries
sudo apt-get install libgtk-3-dev
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
sudo apt-get install libcurl4-openssl-dev
```
### Open the Terminal
```
(from your home dir ~/)

// Clone the NAppGUI repo
git clone --depth 1 https://github.com/frang75/nappgui_src.git nappgui_sdk

// Create a build directory
mkdir nappgui_build
cd nappgui_build

// Generate the Makefiles
cmake -G "Unix Makefiles" ../nappgui_sdk/src

// Build the examples
make

// Run examples in 'demo' and 'howto' folders
./demo/die/Debug/Die
./demo/bricks/Debug/Bricks
./demo/products/Debug/Products
./howto/col2dhello/Debug/Col2dHello
./howto/guihello/Debug/GuiHello
...
```
![Running demo project in Linux](https://nappgui.com/img/start/run_demo_linux.png)

## More info
- [NAppGUI Quick Start](https://nappgui.com/en/start/quick.html)
- [NAppGUI Hello World](https://nappgui.com/en/start/hello.html)
- [Create a new project](https://nappgui.com/en/start/newprj.html)
- [About Portability](https://nappgui.com/en/start/win_mac_linux.html)
