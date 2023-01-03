# NAppGUI
Cross-Platform C SDK. Build portable desktop applications for Windows, macOS and Linux, using just C.

![NAppGUI Stack](https://nappgui.com/img/start/nappgui_stack.png)

## Quick start using the BUSY build system

NAppGUI and it's utility and demo applications can be built ab initio (i.e. with no system requirements 
beyond those of NAppGUI, particularly no Cmake) on all platforms using the BUSY build system. 
Both static and shared library versions of NAppGUI are supported.

Proceed as follows:

1) Open a terminal window which is correctly set up for the intended toolchain; i.e. Visual Studio on Windows
comes with pre-declared macros, such as vcvars32.bat or vcvars64.bat, see https://docs.microsoft.com/en-us/cpp/build/building-on-the-command-line?view=msvc-170; on Mac and Linux a properly configured GCC is usually directly available in the terminal without additional steps.
2) Download https://github.com/rochus-keller/nappgui/archive/refs/heads/BUSY3.zip and unpack
3) Create a build subdirectory in the nappgui directory, e.g. build
4) Download and unpack the BUSY source files to the build directory; you can use this link: https://github.com/rochus-keller/BUSY/archive/refs/heads/main.zip
5) Set the current directory to the build directory.
Then...

...with GCC & CLANG on Linux/Unix:

6) `gcc *.c -O2 -lm -O2 -o lua`

7) `./lua build.lua`

...with GCC & CLANG on Windows:

6) `gcc *.c -O2 -lm -O2 -o lua.exe`

7) `lua build.lua`

...with MSVC on Windows:

6) `cl /O2 /MD /Fe:lua.exe *.c`

7) `lua build.lua`

8) Wait until the build finishes (half a minute); the built libraries are in the output/src subdirectory

## Quick start in Windows

### Prerequisites
- [Visual Studio](https://visualstudio.microsoft.com/vs/)
- [CMake](https://cmake.org/download/)

### Open the Developer Command Prompt
```
git clone --depth 1 https://github.com/frang75/nappgui_src.git
cd nappgui_src
cmake -S ./src -B ./build
cmake --build ./build --config Debug

// Run examples in 'demo' and 'howto' folders
.\build\Debug\bin\Die.exe
.\build\Debug\bin\Bricks.exe
.\build\Debug\bin\Products.exe
.\build\Debug\bin\Col2dHello.exe
.\build\Debug\bin\GuiHello.exe
...
```
![Running demo project in Windows](https://nappgui.com/img/start/run_demo_windows.png)

## Quick start in macOS

### Prerequisites
- [Xcode](https://developer.apple.com/xcode/)
- [CMake](https://cmake.org/download/)

### Open the Terminal
```
git clone --depth 1 https://github.com/frang75/nappgui_src.git
cd nappgui_src
cmake -G Xcode -S ./src -B ./build
cmake --build ./build --config Debug

// Run examples in 'demo' and 'howto' folders
./build/Debug/bin/Die.app/Contents/MacOS/Die
./build/Debug/bin/Bricks.app/Contents/MacOS/Bricks
./build/Debug/bin/Products.app/Contents/MacOS/Products
./build/Debug/bin/Col2dHello.app/Contents/MacOS/Col2dHello
./build/Debug/bin/GuiHello.app/Contents/MacOS/GuiHello
...
```
![Running demo project in macOS](https://nappgui.com/img/start/run_demo_macos.png)

## Quick start in Linux

### Prerequisites
```
// Development tools
sudo apt-get install build-essential
sudo apt-get install git
sudo apt-get install cmake

// Development libraries
sudo apt-get install libgtk-3-dev
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
sudo apt-get install libcurl4-openssl-dev
```
### Open the Terminal
```
git clone --depth 1 https://github.com/frang75/nappgui_src.git
cd nappgui_src
cmake -S ./src -B ./build -DCMAKE_BUILD_CONFIG=Debug
cmake --build ./build -j 4

// Run examples in 'demo' and 'howto' folders
./build/Debug/bin/Die
./build/Debug/bin/Bricks
./build/Debug/bin/Products
./build/Debug/bin/Col2dHello
./build/Debug/bin/GuiHello
...
```
![Running demo project in Linux](https://nappgui.com/img/start/run_demo_linux.png)

## More info
- [NAppGUI Quick Start](https://nappgui.com/en/start/quick.html)
- [NAppGUI Hello World](https://nappgui.com/en/start/hello.html)
- [Create a new project](https://nappgui.com/en/guide/newprj.html)
- [About Portability](https://nappgui.com/en/guide/win_mac_linux.html)
