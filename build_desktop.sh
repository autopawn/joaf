#!/bin/bash -xe

FILES="src/main.c"

gcc $FILES -o joaf -I./raylib/src -L./raylib/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
