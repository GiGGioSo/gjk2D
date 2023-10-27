@ECHO OFF

SETLOCAL

set CFLAGS=-ggdb -Wall -Werror -std=c99
set INCLUDES=-I ./include/
set LIBS=-L ./lib/windows/ -lraylib -lopengl32 -lgdi32 -lwinmm -luser32 -lshell32

set EXE=./bin/gjk.exe
set SOURCES=main.c

RMDIR /s /q bin
MKDIR bin

@ECHO ON
gcc %SOURCES% -o %EXE% %LIBS% %INCLUDES% %CLAGS%
