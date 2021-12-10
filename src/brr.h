#ifndef _BRR_H_
#define _BRR_H_

#ifndef _WINDOWS_
#include <windows.h>
#endif

#include "structs.h"

extern struct sample samp[128];
void decode_samples(WORD *ptrtable);
void free_samples(void);

#endif // _BRR_H_
