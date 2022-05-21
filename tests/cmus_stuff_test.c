#include "../src/cmus_stuff.h"

#include "../src/utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    struct cmus_state cs;

    // Initialize cmus_state {{{
    memset(&cs, 0, sizeof(cs));

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.status      == 0);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0);

    const char * str1 = "Test string 1";
    const char * str2 = "Test string 2";
    const char * str3 = "A test string 3";
    const char * str4 = "Another test string 4!!!";
    const char * str5 = "Just some string 5";

    cs.title       = (char*) xmalloc(strlen(str1)+1);
    cs.file        = (char*) xmalloc(strlen(str2)+1);
    cs.artist      = (char*) xmalloc(strlen(str3)+1);
    cs.album       = (char*) xmalloc(strlen(str4)+1);
    cs.albumartist = (char*) xmalloc(strlen(str5)+1);

    assert(cs.title       != NULL);
    assert(cs.file        != NULL);
    assert(cs.artist      != NULL);
    assert(cs.album       != NULL);
    assert(cs.albumartist != NULL);

    memcpy(cs.title,       str1, strlen(str1)+1);
    memcpy(cs.file,        str2, strlen(str2)+1);
    memcpy(cs.artist,      str3, strlen(str3)+1);
    memcpy(cs.album,       str4, strlen(str4)+1);
    memcpy(cs.albumartist, str5, strlen(str5)+1);

    assert(strcmp(cs.title,       str1) == 0);
    assert(strcmp(cs.file,        str2) == 0);
    assert(strcmp(cs.artist,      str3) == 0);
    assert(strcmp(cs.album,       str4) == 0);
    assert(strcmp(cs.albumartist, str5) == 0);

    // Freeing
    free_cmus_state(&cs);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL); /*}}}*/

    // Parsing lines output by cmus-remote

    // Cmus not running {{{
    memset(&cs, 0, sizeof(cs));

    cs.status = -1;
    const char * str6 = "cmus-remote: cmus is not running";
    _cmus_parse_line(&cs, str6);

    assert(cs.status == 0);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0);

    free_cmus_state(&cs); /*}}}*/

    // Tag: title {{{
    memset(&cs, 0, sizeof(cs));

    const char * str7 = "tag title some nice title";
    _cmus_parse_line(&cs, str7);

    assert(cs.title != NULL);
    assert(strcmp(cs.title, "some nice title") == 0);

    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.status      == 0);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0);

    free_cmus_state(&cs); /*}}}*/

    // Tag: artist {{{
    memset(&cs, 0, sizeof(cs));

    const char * str8 = "tag artist A very talented artist!!#";
    _cmus_parse_line(&cs, str8);

    assert(cs.artist != NULL);
    assert(strcmp(cs.artist, "A very talented artist!!#") == 0);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.status      == 0);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0);

    free_cmus_state(&cs); /*}}}*/

    // Tag: album {{{
    memset(&cs, 0, sizeof(cs));

    const char * str9 = "tag album Some nice hardstyle VOL//1";
    _cmus_parse_line(&cs, str9);

    assert(cs.album != NULL);
    assert(strcmp(cs.album, "Some nice hardstyle VOL//1") == 0);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.status      == 0);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0);

    free_cmus_state(&cs); /*}}}*/

    // Tag: albumartist {{{
    memset(&cs, 0, sizeof(cs));

    const char * str10 = "tag albumartist Som3 F#uckng Ga1 3x";
    _cmus_parse_line(&cs, str10);

    assert(cs.albumartist != NULL);
    assert(strcmp(cs.albumartist, "Som3 F#uckng Ga1 3x") == 0);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.status      == 0);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0);

    free_cmus_state(&cs); /*}}}*/

    // Tag: track {{{
    memset(&cs, 0, sizeof(cs));

    cs.tracknumber = -1;
    const char * str11 = "tag tracknumber 324";
    _cmus_parse_line(&cs, str11);

    assert(cs.tracknumber == 324);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.status      == 0);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);

    free_cmus_state(&cs); /*}}}*/

    // Status playing {{{
    memset(&cs, 0, sizeof(cs));

    cs.status = -1;
    const char * str12 = "status playing";
    _cmus_parse_line(&cs, str12);

    assert(cs.status      == 1);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0);

    free_cmus_state(&cs); /*}}}*/

    // Status paused {{{
    memset(&cs, 0, sizeof(cs));

    cs.status = -1;
    const char * str13 = "status paused";
    _cmus_parse_line(&cs, str13);

    assert(cs.status      == 0);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0);

    free_cmus_state(&cs); /*}}}*/

    // Position {{{
    memset(&cs, 0, sizeof(cs));

    cs.position = -1;
    const char * str14 = "position 3450";
    _cmus_parse_line(&cs, str14);

    assert(cs.position == 3450);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.status      == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0);

    free_cmus_state(&cs); /*}}}*/

    // Duration {{{
    memset(&cs, 0, sizeof(cs));

    cs.duration = -1;
    const char * str15 = "duration 9845";
    _cmus_parse_line(&cs, str15);

    assert(cs.duration == 9845);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.status      == 0);
    assert(cs.position    == 0);
    assert(cs.tracknumber == 0);

    free_cmus_state(&cs); /*}}}*/

    // File {{{
    memset(&cs, 0, sizeof(cs));

    const char * str16 = "file /some/directory/and/file - nice name.opus";
    _cmus_parse_line(&cs, str16);

    assert(cs.file != NULL);
    assert(strcmp(cs.file, "/some/directory/and/file - nice name.opus") == 0);

    assert(cs.title       == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.status      == 0);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0);

    free_cmus_state(&cs); /*}}}*/

    // TODO:
    // create_status
}
