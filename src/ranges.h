#ifndef _RANGES_H_
#include "structs.h"

#define _RANGES_H_
#define AREA_END -4
#define AREA_NOT_IN_FILE -3
#define AREA_NON_SPC -2
#define AREA_FREE -1
extern int area_count;
extern struct area { int address, pack; } *areas;
extern void init_areas(void);
extern void change_range(int start, int end, int from, int to);
extern int check_range(int start, int end, int pack);

#endif // _RANGES_H_
