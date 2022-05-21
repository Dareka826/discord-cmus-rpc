#include "../src/cmus_stuff.h"

#include "../src/utils.h"
#include "../src/discord_stuff.h"
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
    assert(strcmp(cs.title, str7+10) == 0);

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
    assert(strcmp(cs.artist, str8+11) == 0);

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
    assert(strcmp(cs.album, str9+10) == 0);

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
    assert(strcmp(cs.albumartist, str10+16) == 0);

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
    _cmus_parse_line(&cs, "tag tracknumber 324");

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
    _cmus_parse_line(&cs, "status playing");

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
    _cmus_parse_line(&cs, "status paused");

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
    _cmus_parse_line(&cs, "position 3450");

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
    _cmus_parse_line(&cs, "duration 9845");

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

    const char * str11 = "file /some/directory/and/file - nice name.opus";
    _cmus_parse_line(&cs, str11);

    assert(cs.file != NULL);
    assert(strcmp(cs.file, str11+5) == 0);

    assert(cs.title       == NULL);
    assert(cs.artist      == NULL);
    assert(cs.album       == NULL);
    assert(cs.albumartist == NULL);
    assert(cs.status      == 0);
    assert(cs.position    == 0);
    assert(cs.duration    == 0);
    assert(cs.tracknumber == 0);

    free_cmus_state(&cs); /*}}}*/

    // Create status
    struct presence_state ps;

    // State: <file> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    const char * str12 = "file /some/nice/dir/##. artist - nice file.opus";
    _cmus_parse_line(&cs, str12);
    create_status(&cs, &ps);

    assert(strcmp(ps.state, str12+20) == 0);

    free_presence_state(&ps);
    free_cmus_state(&cs); /*}}}*/

    // State: <title> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    const char * str13 = "tag title <nice title>";
    _cmus_parse_line(&cs, str13);
    create_status(&cs, &ps);

    assert(strcmp(ps.state, str13+10) == 0);

    free_presence_state(&ps);
    free_cmus_state(&cs); /*}}}*/

    // State: <tracknumber>. <title> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    _cmus_parse_line(&cs, "tag tracknumber 8");
    _cmus_parse_line(&cs, str13);
    create_status(&cs, &ps);

    assert(strcmp(ps.state, "08. <nice title>") == 0);

    free_presence_state(&ps);
    free_cmus_state(&cs); /*}}}*/

    // State: <tracknumber>. <title> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    _cmus_parse_line(&cs, "tag tracknumber 346");
    _cmus_parse_line(&cs, str13);
    create_status(&cs, &ps);

    assert(strcmp(ps.state, "346. <nice title>") == 0);

    free_presence_state(&ps);
    free_cmus_state(&cs); /*}}}*/

    // State: <artist> - <title> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    const char * str14 = "tag artist <artist 348u>";
    _cmus_parse_line(&cs, str14);
    _cmus_parse_line(&cs, str13);
    create_status(&cs, &ps);

    assert(strcmp(ps.state, "<artist 348u> - <nice title>") == 0);

    free_presence_state(&ps);
    free_cmus_state(&cs); /*}}}*/

    // State: <tracknumber>. <artist> - <title> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    _cmus_parse_line(&cs, "tag tracknumber 56");
    _cmus_parse_line(&cs, str14);
    _cmus_parse_line(&cs, str13);
    create_status(&cs, &ps);

    assert(strcmp(ps.state, "56. <artist 348u> - <nice title>") == 0);

    free_presence_state(&ps);
    free_cmus_state(&cs); /*}}}*/

    // Details: <empty> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    create_status(&cs, &ps);

    assert(strcmp(ps.details, "") == 0);

    free_presence_state(&ps);
    free_cmus_state(&cs); /*}}}*/

    // Details: <album> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    const char * str15 = "tag album \"The greatest album ever #02\"*";
    _cmus_parse_line(&cs, str15);
    create_status(&cs, &ps);

    assert(strcmp(ps.details, str15+10) == 0);

    free_presence_state(&ps);
    free_cmus_state(&cs); /*}}}*/

    // Details: [<albumartist>] <album> {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    _cmus_parse_line(&cs, str15);
    _cmus_parse_line(&cs, "tag albumartist Hard J-Pop Collab");
    create_status(&cs, &ps);

    assert(strcmp(ps.details, "[Hard J-Pop Collab] \"The greatest album ever #02\"*") == 0);

    free_presence_state(&ps);
    free_cmus_state(&cs); /*}}}*/

    // Time left {{{
    memset(&cs, 0, sizeof(cs));
    memset(&ps, 0, sizeof(ps));

    _cmus_parse_line(&cs, "position 34");
    _cmus_parse_line(&cs, "duration 547");
    create_status(&cs, &ps);

    assert(ps.time_left == (547 - 34));

    free_presence_state(&ps);
    free_cmus_state(&cs); /*}}}*/

    return EXIT_SUCCESS;
}
