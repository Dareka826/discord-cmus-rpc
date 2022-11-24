# vim: ft=sh
redo-ifchange ../discord-rpc/build.py
redo-ifchange ../discord-rpc/src/*

(
cd ../discord-rpc
./build.py libs
) >&2

cp ../discord-rpc/builds/linux-static/src/libdiscord-rpc.a "${3}"
