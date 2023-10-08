@ECHO OFF

SETLOCAL

set CFLAGS=-ggdb -Wall -Werror -std=c99
set INCLUDES=-I ./raylib-4.5.0/include/
set LIBS=-L ./raylib-4.5.0/lib/ -lraylib -lopengl32 -lgdi32 -lwinmm -luser32 -lshell32

set EXE=gjk.exe
set SOURCES=main.c

@ECHO ON
gcc %SOURCES% -o %EXE% %LIBS% %INCLUDES% %CLAGS%
