#ifndef _TEXT_H_
#define _TEXT_H_

#ifndef _WINDOWS_
#include <windows.h>
#endif

#include "structs.h"

int calc_track_size_from_text(char *p);
BOOL text_to_track(char *str, struct track *t, BOOL is_sub);
int text_length(BYTE *start, BYTE *end);
void track_to_text(char *out, BYTE *track, int size);

#endif // text.h