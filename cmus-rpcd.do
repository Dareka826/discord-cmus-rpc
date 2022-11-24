redo-ifchange ./config.sh
. ./config.sh

OBJ="./build/colors.o ./build/utils.o ./src/snippets/build/arena/arena.o ./build/discord_stuff.o ./build/cmus_stuff.o ./build/main.o"
redo-ifchange ${OBJ}

LDFLAGS="${LDFLAGS} -L./discord-rpc/builds/linux-static/src -ldiscord-rpc"

# Link
${CXX:-g++} ${OBJ} ${CFLAGS} ${LDFLAGS} -o "${3}"
