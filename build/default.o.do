redo-ifchange ../config.sh
. ../config.sh

redo-ifchange "../src/${2}.c"

# Compile
${CC:-gcc} -c "../src/${2}.c" ${CFLAGS} -MD -MF "${2}.d" -o "${3}"

# Depend on dependencies
read DEPS <"${2}.d"
redo-ifchange ${DEPS#*:}
