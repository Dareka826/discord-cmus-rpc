redo-ifchange ../config.sh
. ../config.sh

redo-ifchange "../src/${2}.c"

VERSION="$(git describe --long --tags | sed 's/\([^-]*-g\)/r\1/;s/-/./')"

# Compile
${CC:-gcc} -c "../src/${2}.c" ${CFLAGS} -D_VERSION="\"${VERSION}\"" -MD -MF "${2}.d" -o "${3}"

# Depend on dependencies
read DEPS <"${2}.d"
redo-ifchange ${DEPS#*:}
