redo-ifchange ../config.sh
. ../config.sh

redo-ifchange "${2}.o"

# Compile
${CC:-gcc} -c "../tests/${2}_test.c" ${CFLAGS} -MD -MF "${2}.test.d" -o "${2}.test.o"

# Depend on dependencies
read DEPS <"${2}.test.d"
redo-ifchange ${DEPS#*:}

# Additional obj files (could be autodetected, but there's no reason)
OBJ="colors.o utils.o discord_stuff.o cmus_stuff.o ../src/snippets/build/arena/arena.o"
redo-ifchange ${OBJ}

LDFLAGS="${LDFLAGS} -L../discord-rpc/builds/linux-static/src -ldiscord-rpc"

# Link
${CXX:-g++} ${OBJ} "${2}.test.o" ${LDFLAGS} -o "${3}"
