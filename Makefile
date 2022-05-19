CC = clang
CFLAGS = -g -Wall -xc
LDFLAGS = 

cmus-rpcd: main.c
	${CC} main.c -o cmus-rpcd ${CFLAGS} ${LDFLAGS}
