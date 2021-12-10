#ifndef _CONSTANTS_H_

#define _CONSTANTS_H_

// EarthBound related constants
#define NUM_SONGS 0xBF
#define NUM_PACKS 0xA9
#define BGM_PACK_TABLE 0x4F70A
#define PACK_POINTER_TABLE 0x4F947
#define SONG_POINTER_TABLE 0x26298C

// other constants and stuff
#define MAX_TITLE_LEN 60
#define MAX_TITLE_LEN_STR "60"
#define WM_ROM_OPENED WM_USER
#define WM_ROM_CLOSED WM_USER+1
#define WM_SONG_IMPORTED WM_USER+2
#define WM_SONG_LOADED WM_USER+3
#define WM_SONG_NOT_LOADED WM_USER+4
#define WM_PACKS_SAVED WM_USER+5

#endif // _CONSTANTS_H_