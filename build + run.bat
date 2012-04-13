@echo off
echo Build Started...

windres -o temp/res.o src/resource.rc
gcc src/main.c -c -o temp/main.o
gcc -o build/attract.exe temp/main.o temp/res.o -lopengl32 -mwindows

build\attract.exe
