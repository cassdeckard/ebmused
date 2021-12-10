#ifndef _SONGED_H_
#ifndef _WINDOWS_
#include <windows.h>
#endif
#include "structs.h"

#define _SONGED_H_
void order_insert(int pos, int pat);
struct track *pattern_insert(int pat);
void pattern_delete(int pat);
BOOL split_pattern(int pos);
BOOL join_patterns(void);
int create_sub(BYTE *start, BYTE *end, int *count);
void order_delete(int pos);

#endif // _SONGED_H_