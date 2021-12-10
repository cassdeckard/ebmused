#ifndef _SONG_H_
#ifndef _WINDOWS_
#include <windows.h>
#endif

#define _SONG_H_
extern char *decomp_error;
BOOL validate_track(BYTE *data, int size, BOOL is_sub);
int compile_song(struct song *s);
BOOL decompile_song(struct song *s, int start_addr, int end_addr);
void free_song(struct song *s);

#endif // _SONG_H_