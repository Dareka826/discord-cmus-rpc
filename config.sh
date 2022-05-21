CC="clang++"
CFLAGS="-g -O2 -Wall"
CFLAGS="-xc++ -Wno-writable-strings" # Have to compile as C++, because discord-rpc is
CFLAGS="${CFLAGS} -DMEM_INFO_D=1" # For extra memory information
LDFLAGS="-L./discord-rpc/builds/linux-static/src -ldiscord-rpc"
