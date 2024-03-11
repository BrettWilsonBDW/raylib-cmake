@echo off
REM Turn off command echoing for cleaner output

pushd %~dp0
REM Change the current directory to the script's directory

mkdir build-web
REM Create a directory named 'build-web' if it doesn't exist

xcopy /E /I /Y ".\assets\" ".\build-web\assets\"
REM Copy the contents of the 'assets' directory to 'build-web/assets/'
REM /E: Copy directories and subdirectories, including empty ones
REM /I: Assume the destination is a directory if more than one file is being copied
REM /Y: Suppress the prompt to confirm overwriting files

cd build-web
REM Change the current directory to 'build-web'


@REM @REM use the below if you have set up emscripten_set_main_loop()
@REM call emcmake cmake .. -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3" -DCMAKE_EXECUTABLE_SUFFIX=".html"
@REM @REM else use the below if you have not set up emscripten_set_main_loop()
@REM @REM call emcmake cmake .. -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3" -DCMAKE_EXECUTABLE_SUFFIX=".html" -DASYNCIFY=1


IF "%1"=="async" (
    echo using asyncify
    call emcmake cmake .. -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3" -DCMAKE_EXECUTABLE_SUFFIX=".html" -DASYNCIFY=1
) ELSE (
    call emcmake cmake .. -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3" -DCMAKE_EXECUTABLE_SUFFIX=".html"
)
REM Call 'emcmake cmake' to configure the project for Emscripten/Web
REM -DPLATFORM=Web: Set the platform to Web
REM -DCMAKE_BUILD_TYPE=Release: Build in Release mode
REM -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3": Link GLFW library
REM -DCMAKE_EXECUTABLE_SUFFIX=".html": Set the executable suffix to .html

call emmake make
REM Call 'emmake make' to build the project using Emscripten
