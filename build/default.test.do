redo-ifchange ../config.sh
. ../config.sh

redo-ifchange "${2}.o"

# Compile
${CC:-gcc} -c "../tests/${2}_test.c" ${CFLAGS} -MD -MF "${2}.test.d" -o "${2}.test.o"

# Depend on dependencies
read DEPS <"${2}.test.d"
redo-ifchange ${DEPS#*:}

# Additional obj files
OBJ=""
[ "${2}" != "colors"        ] && OBJ="${OBJ} colors.o"
[ "${2}" != "utils"         ] && OBJ="${OBJ} utils.o"
[ "${2}" != "discord_stuff" ] && OBJ="${OBJ} discord_stuff.o"
[ "${2}" != "cmus_stuff"    ] && OBJ="${OBJ} cmus_stuff.o"

LDFLAGS="${LDFLAGS} -L../discord-rpc/builds/linux-static/src -ldiscord-rpc"

# Link
${CXX:-g++} ${OBJ} "${2}.test.o" ${LDFLAGS} -o "${3}"
