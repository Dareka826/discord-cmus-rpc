#include "../src/cmus_stuff.h"

#include "../src/utils.h"
#include "../src/discord_stuff.h"
#include "../src/snippets/arena/arena.h"

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

    const char * const str1 = "Test string 1";
    const char * const str2 = "Test string 2";
    const char * const str3 = "A test string 3";
    const char * const str4 = "Another test string 4!!!";
    const char * const str5 = "Just some string 5";

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

    nfree(cs.title);
    nfree(cs.file);
    nfree(cs.artist);
    nfree(cs.album);
    nfree(cs.albumartist); /*}}}*/

    // Parsing lines output by cmus-remote
    Arena *a = create_arena();

    // Cmus not running {{{
    memset(&cs, 0, sizeof(cs));

    cs.status = -1;
    const char * const str6 = "cmus-remote: cmus is not running";
    _cmus_parse_line(a, &cs, str6);

    assert(cs.status == 0);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0); /*}}}*/

    // Tag: title {{{
    memset(&cs, 0, sizeof(cs));

    const char * const str7 = "tag title some nice title";
    _cmus_parse_line(a, &cs, str7);

    assert(cs.title != NULL);
    assert(strcmp(cs.title, str7+10) == 0);

    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.status      == 0);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0); /*}}}*/

    // Tag: artist {{{
    memset(&cs, 0, sizeof(cs));

    const char * const str8 = "tag artist A very talented artist!!#";
    _cmus_parse_line(a, &cs, str8);

    assert(cs.artist != NULL);
    assert(strcmp(cs.artist, str8+11) == 0);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.status      == 0);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0); /*}}}*/

    // Tag: album {{{
    memset(&cs, 0, sizeof(cs));

    const char * const str9 = "tag album Some nice hardstyle VOL//1";
    _cmus_parse_line(a, &cs, str9);

    assert(cs.album != NULL);
    assert(strcmp(cs.album, str9+10) == 0);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.status      == 0);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0); /*}}}*/

    // Tag: albumartist {{{
    memset(&cs, 0, sizeof(cs));

    const char * const str10 = "tag albumartist Som3 F#uckng Ga1 3x";
    _cmus_parse_line(a, &cs, str10);

    assert(cs.albumartist != NULL);
    assert(strcmp(cs.albumartist, str10+16) == 0);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.status      == 0);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0); /*}}}*/

    // Tag: track {{{
    memset(&cs, 0, sizeof(cs));

    cs.tracknumber = -1;
    _cmus_parse_line(a, &cs, "tag tracknumber 324");

    assert(cs.tracknumber == 324);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.status      == 0);
    assert(cs.position    == 0);
    assert(cs.duration    == 0); /*}}}*/

    // Status playing {{{
    memset(&cs, 0, sizeof(cs));

    cs.status = -1;
    _cmus_parse_line(a, &cs, "status playing");

    assert(cs.status      == 1);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0); /*}}}*/

    // Status paused {{{
    memset(&cs, 0, sizeof(cs));

    cs.status = -1;
    _cmus_parse_line(a, &cs, "status paused");

    assert(cs.status      == 0);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0); /*}}}*/

    // Position {{{
    memset(&cs, 0, sizeof(cs));

    cs.position = -1;
    _cmus_parse_line(a, &cs, "position 3450");

    assert(cs.position == 3450);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.status      == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0); /*}}}*/

    // Duration {{{
    memset(&cs, 0, sizeof(cs));

    cs.duration = -1;
    _cmus_parse_line(a, &cs, "duration 9845");

    assert(cs.duration == 9845);

    assert(cs.title       == NULL);
    assert(cs.file        == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.status      == 0);
    assert(cs.position    == 0);
    assert(cs.tracknumber == 0); /*}}}*/

    // File {{{
    memset(&cs, 0, sizeof(cs));

    const char * const str11 = "file /some/directory/and/file - nice name.opus";
    _cmus_parse_line(a, &cs, str11);

    assert(cs.file != NULL);
    assert(strcmp(cs.file, str11+5) == 0);

    assert(cs.title       == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.status      == 0);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0); /*}}}*/

    // Create status
    struct presence_state ps;

    // State: <file> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    const char * const str12 = "file /some/nice/dir/##. artist - nice file.opus";
    _cmus_parse_line(a, &cs, str12);
    create_status(a, &cs, &ps);

    assert(strcmp(ps.state, str12+20) == 0); /*}}}*/

    // State: <title> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    const char * const str13 = "tag title <nice title>";
    _cmus_parse_line(a, &cs, str13);
    create_status(a, &cs, &ps);

    assert(strcmp(ps.state, str13+10) == 0); /*}}}*/

    // State: <tracknumber>. <title> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    _cmus_parse_line(a, &cs, "tag tracknumber 8");
    _cmus_parse_line(a, &cs, str13);
    create_status(a, &cs, &ps);

    assert(strcmp(ps.state, "08. <nice title>") == 0); /*}}}*/

    // State: <tracknumber>. <title> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    _cmus_parse_line(a, &cs, "tag tracknumber 346");
    _cmus_parse_line(a, &cs, str13);
    create_status(a, &cs, &ps);

    assert(strcmp(ps.state, "346. <nice title>") == 0); /*}}}*/

    // State: <artist> - <title> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    const char * const str14 = "tag artist <artist 348u>";
    _cmus_parse_line(a, &cs, str14);
    _cmus_parse_line(a, &cs, str13);
    create_status(a, &cs, &ps);

    assert(strcmp(ps.state, "<artist 348u> - <nice title>") == 0); /*}}}*/

    // State: <tracknumber>. <artist> - <title> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    _cmus_parse_line(a, &cs, "tag tracknumber 56");
    _cmus_parse_line(a, &cs, str14);
    _cmus_parse_line(a, &cs, str13);
    create_status(a, &cs, &ps);

    assert(strcmp(ps.state, "56. <artist 348u> - <nice title>") == 0); /*}}}*/

    // Details: <empty> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    create_status(a, &cs, &ps);

    assert(strcmp(ps.details, "") == 0); /*}}}*/

    // Details: <album> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    const char * const str15 = "tag album \"The greatest album ever #02\"*";
    _cmus_parse_line(a, &cs, str15);
    create_status(a, &cs, &ps);

    assert(strcmp(ps.details, str15+10) == 0); /*}}}*/

    // Details: [<albumartist>] <album> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    _cmus_parse_line(a, &cs, str15);
    _cmus_parse_line(a, &cs, "tag albumartist Hard J-Pop Collab");
    create_status(a, &cs, &ps);

    assert(strcmp(ps.details, "[Hard J-Pop Collab] \"The greatest album ever #02\"*") == 0); /*}}}*/

    // Time left {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    _cmus_parse_line(a, &cs, "position 34");
    _cmus_parse_line(a, &cs, "duration 547");
    create_status(a, &cs, &ps);

    assert(ps.time_left == (547 - 34)); /*}}}*/

    nfree_arena(a);
    return EXIT_SUCCESS;
}
