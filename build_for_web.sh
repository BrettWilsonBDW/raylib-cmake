#!/bin/bash

# Change the current directory to the script's directory
cd "$(dirname "$0")"

# Create a directory named 'build-web' if it doesn't exist
mkdir -p build-web

# Copy the contents of the 'assets' directory to 'build-web/assets/'
cp -R assets/ build-web/assets/

# Change the current directory to 'build-web'
cd build-web

if [ "$1" == "async" ]; then
    echo "Using asyncify"
    emcmake cmake .. -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3" -DCMAKE_EXECUTABLE_SUFFIX=".html" -DASYNCIFY=1
else
    emcmake cmake .. -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3" -DCMAKE_EXECUTABLE_SUFFIX=".html"
fi

# Build the project using Emscripten
emmake make
