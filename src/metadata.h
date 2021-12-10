#ifndef _METADATA_H_
#ifndef _WINDOWS_
#include <windows.h>
#endif
#include "constants.h"

#define _METADATA_H_
extern char *bgm_title[NUM_SONGS];
extern BOOL metadata_changed;
#ifdef EOF
extern FILE *orig_rom;
extern int orig_rom_offset;
#endif
extern char *orig_rom_filename;
extern const char *const bgm_orig_title[NUM_SONGS];
void load_metadata(void);
void save_metadata(void);
void free_metadata(void);

#endif // _METADATA_H_