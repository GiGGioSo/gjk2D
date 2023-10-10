
CFLAGS="-ggdb -Wall -Werror -std=c99"
INCLUDES="-I ./include/"
LIBS="-L./lib/linux/ -lm -ldl -lraylib"

EXE="./bin/gjk"
SOURCES="main.c"

set -xe

rm -rf bin
mkdir bin

gcc $SOURCES -o $EXE $LIBS $INCLUDES $CLAGS
