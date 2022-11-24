# discord-cmus-rpc

Display current playing song in cmus on Discord with RPC

## Build

### Requirements

- The same as for discord-rpc.
- A tool which understands the redo build system.

### Commands

```sh
git clone https://github.com/Dareka826/discord-cmus-rpc
cd discord-cmus-rpc
git submodule update --init
redo
```

## Running
While the `cmus-rpcd` binary is running it will check cmus status every second and update the rpc accordingly.

## How the information is displayed

### State

If `tracknumber`, `artist` and `title` are present:
`<tracknumber>. <artist> - <title>`

If `artist` and `title` are present:
`<artist> - <title>`

If `tracknumber` and `title` are present:
`<tracknumber>. <title>`

If only `title` is present:
`<title>`

If none of the above are present, use the filename:
`<file>`

### Details

If `album` and `albumartist` are present:
`[<albumartist>] <album>`

If only `album` is present:
`<album>`

If neither are present, nothing is displayed in the details.
