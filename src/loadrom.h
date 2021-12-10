#ifndef _LOADROM_H_
#ifndef _WINDOWS_
#include <windows.h>
#endif
#include "constants.h"
#include "structs.h"

#define _LOADROM_H_
#ifdef EOF
extern FILE *rom;
#endif
extern int rom_size;
extern int rom_offset;
extern char *rom_filename;
extern unsigned char pack_used[NUM_SONGS][3];
extern unsigned short song_address[NUM_SONGS];
extern struct pack rom_packs[NUM_PACKS];
extern struct pack inmem_packs[NUM_PACKS];
BOOL close_rom(void);
BOOL open_rom(char *filename, BOOL readonly);
BOOL open_orig_rom(char *filename);

#endif // _LOADROM_H_