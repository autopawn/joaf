#!/bin/bash -xe

# cd to the directory where this script is
cd "$(dirname "$0")"

# Clone raylib repo
git clone https://github.com/raysan5/raylib.git || true


# make for desktop
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP GRAPHICS=GRAPHICS_API_OPENGL_21


