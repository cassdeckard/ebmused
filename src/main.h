#ifndef _MAIN_H_
#define _MAIN_H_

#ifndef _WINDOWS_
#include <windows.h>
#endif

#include "structs.h"

extern BYTE packs_loaded[3];
extern int current_block;
extern int octave;
extern int midiDevice;
extern struct song cur_song;
extern struct song_state pattop_state, state;
extern HINSTANCE hinstance;
extern HWND hwndMain;
#ifdef CreateWindow
extern HMENU hmenu, hcontextmenu;
extern HFONT hfont;
#endif
#define NUM_TABS 4
extern HWND tab_hwnd[NUM_TABS];
#define hwndBGMList tab_hwnd[0]
#define hwndInstruments tab_hwnd[1]
#define hwndEditor tab_hwnd[2]
#define hwndPackList tab_hwnd[3]
BOOL get_original_rom(void);
BOOL save_all_packs(void);

#endif // _MAIN_H_