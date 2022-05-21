redo-ifchange ./config.sh
. ./config.sh

OBJ="./build/colors.o ./build/main.o"
redo-ifchange ${OBJ}

${CC} ${OBJ} ${CFLAGS:-} ${LDFLAGS:-} -o $3
