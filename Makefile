CC = clang++
CFLAGS = -g -O2 -Wall -xc++ -L./discord-rpc/builds/linux-static/src
LDFLAGS = -ldiscord-rpc

debug: main.c ./discord-rpc/builds/linux-static/src/libdiscord-rpc.a
	${CC} -DMEM_INFO_D=1 main.c -o cmus-rpcd ${CFLAGS} ${LDFLAGS}

release: main.c ./discord-rpc/builds/linux-static/src/libdiscord-rpc.a
	${CC} main.c -o cmus-rpcd ${CFLAGS} ${LDFLAGS}
