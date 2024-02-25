# Raylib CMake Template

This template simplifies the setup process for raylib using CMake and CMake FetchContent. It automatically fetches and configures raylib along with a few essential game development libraries. Enjoy a hassle-free start to your game development journey with raylib using this straightforward template. Happy coding!

### libraries supported

- [raylib](https://github.com/raysan5/raylib) - A simple and easy-to-use library to enjoy videogames programming.
- [ImGui](https://github.com/ocornut/imgui) - Dear ImGui: Bloat-free Graphical User interface for C++ with minimal dependencies.
- [LDtkLoader](https://github.com/Madour/LDtkLoader) - Used to load and help
  with drawing a map made with the awesome [LDtk](https://ldtk.io/).
- [box2d](https://github.com/erincatto/box2d) - Ubiquitous and easy to use 2D
  physics engine.
- [fmt](https://github.com/fmtlib/fmt) - Logging and string formatting library
  that makes your life much easier.
- [bullet3](https://github.com/bulletphysics/bullet3) - 3d physics engine.
- [reactphysics3d](https://github.com/DanielChappuis/ReactPhysics3D) - Open source C++ physics engine library in 3D.

### How to use this template

#### Prerequisites
- [cmake](https://cmake.org/)
- [mingw](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/8.1.0/threads-posix/seh/) or [Visual Studio](https://visualstudio.microsoft.com/) with Desktop development with c++
- [Emscripten](https://github.com/emscripten-core/emsdk) (for web build)


### Choosing What libraries to use
```bash
## In CmakeLists.txt edit the following

# raylib
set(useRaylib ON)
set(useMinimalRaylibModules OFF) # used mostly with apps with no need for game modules

# imgui files
set(useImGui OFF)
set(noAssets OFF) # disable asset macros used with mostly with gui programs without assets

# ldtk loader
set(useLDtkLoader OFF)

# fmt library
set(useFmt OFF)

# box2d 2d physics
set(useBox2d OFF)

# bullet physics 3d
set(useBullet3 OFF)

# reactphysics 3d
set(useReactPhysics3D OFF)

```

## Building for Desktop

```bash
## Clone the repository
git clone https://github.com/tupini07/raylib-cmake.git

## Navigate to the project directory
cd raylib-cmake

## Create a build directory
mkdir build

## Navigate to the build directory
cd build

## Run CMake
cmake -DCMAKE_BUILD_TYPE=Release ..

## Build the project using Visual Studio (you can also use Ninja or MinGW)
cmake --build . --config Release

## The executable is found in the build directory either in its root or in debug/release file if using msvc
```

## Building for the Web

```bash
## Ensure Emscripten is installed and configured:
## on windows
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk.bat install latest
./emsdk.bat activate latest --permanent

## add emsdk to path ex: C:\Program Files\emsdk

## on linux
cd /usr/local
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest --permanent

## add emsdk to path

## Navigate to the project directory
cd raylib-cmake

## Run the build script for the web
.\build_for_web.bat async

## If emscripten_set_main_loop_arg is set up then just use:
.\build_for_web.bat

## This give much better performance in the web
## Example of how to use emscripten_set_main_loop_arg below
}
```
##### emscripten_set_main_loop_arg example

```c++
void ClassName::MainLoopHelper(void *userData)
{
    ClassName *className = static_cast<ClassName *>(userData);

    ClassName->Draw(); // call your game functions here
}

// main loop
void CLassName::Run()
{
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop_arg(MainLoopHelper, this, 0, 1);
#else
    while (!WindowShouldClose())
    {
        MainLoopHelper(this);
    }
#endif

// replace ClassName with the name of your class and function names with your functions
```

### Troubleshooting

```bash
## If missing package issues arise while trying to build in linx you may need to install or update the following:

sudo apt install libx11-dev
sudo apt install libxrandr-dev
sudo apt install libxinerama-dev
sudo apt install libxcursor-dev
sudo apt install libxi-dev

## If missing opengl dir. install or update
sudo apt install libgl1-mesa-dev
```

## Acknowledgments

- [raylib-cpp-cmake-template](https://github.com/tupini07/raylib-cpp-cmake-template) - Raylib cmake starting point template by [tupini07](https://github.com/tupini07).
