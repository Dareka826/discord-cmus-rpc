redo-ifchange ./config.sh
. ./config.sh

OBJ="./build/colors.o ./build/utils.o ./build/discord_stuff.o ./build/cmus_stuff.o ./build/main.o"
redo-ifchange ${OBJ}

${CC} ${OBJ} ${CFLAGS:-} ${LDFLAGS:-} -o $3
