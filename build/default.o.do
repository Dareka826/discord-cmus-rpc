redo-ifchange ../config.sh
. ../config.sh

redo-ifchange ../src/$2.c

${CC} ${CFLAGS:-} -MD -MF $2.d -c -o $3 ../src/$2.c

read DEPS <$2.d
redo-ifchange ${DEPS#*:}
