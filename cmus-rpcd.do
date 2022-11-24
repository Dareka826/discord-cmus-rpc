redo-ifchange ./config.sh
. ./config.sh

OBJ=""
OBJ+=" ./build/colors.o"
OBJ+=" ./build/utils.o"
OBJ+=" ./src/snippets/build/arena/arena.o"
OBJ+=" ./build/discord_stuff.o"
OBJ+=" ./build/cmus_stuff.o"
OBJ+=" ./build/main.o"

redo-ifchange ${OBJ}

LDFLAGS="${LDFLAGS} -L./build -l:libdiscord-rpc.a"

# Link
${CXX:-g++} ${OBJ} ${CFLAGS} ${LDFLAGS} -o "${3}"
