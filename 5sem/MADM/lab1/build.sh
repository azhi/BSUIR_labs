#! /bin/bash
gcc main.cpp -o lab1 -std=c++0x $(sdl-config --cflags --libs) -lSDL_gfx
