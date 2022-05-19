# discord-cmus-rpc

Display current song in cmus as RPC on Discord

## Build
```sh
git clone https://github.com/Dareka826/discord-cmus-rpc
cd discord-cmus-rpc
git submodule update --init
cd discord-rpc
./build.py libs
cd ..
make```

## Running
Just keep the resulting binary `cmus-rpcd` running and it will update the rpc status.

## How the information is displayed
```
[<album_artist>] <album>
<track>. <artist> - <title>```
