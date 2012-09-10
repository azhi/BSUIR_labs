#! /bin/bash
gcc main.cpp -o lab2 -std=c++0x $(sdl-config --cflags --libs) -lSDL_gfx
