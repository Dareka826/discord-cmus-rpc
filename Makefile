CC = clang++
CFLAGS = -g -Wall -xc++ -L./discord-rpc/builds/linux-static/src
LDFLAGS = -ldiscord-rpc

cmus-rpcd: main.c ./discord-rpc/builds/linux-static/src/libdiscord-rpc.a
	${CC} main.c -o cmus-rpcd ${CFLAGS} ${LDFLAGS}
