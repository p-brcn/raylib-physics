#!/bin/sh

set -xe

clang -Wall -Wextra -lraylib -o main main.c 
clang -shared -fPIC -o libgame.so module_game.c
