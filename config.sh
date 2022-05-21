CC="clang++"
CFLAGS="-g -O2 -Wall -xc++"
CFLAGS="${CFLAGS} -DMEM_INFO_D=1" # For extra memory information
LDFLAGS="-L./discord-rpc/builds/linux-static/src -ldiscord-rpc"
