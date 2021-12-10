#ifndef _TRACKER_H_
#ifndef _WINDOWS_
#include <windows.h>
#endif

#define _TRACKER_H_
extern HWND hwndTracker;
void tracker_scrolled(void);
void load_pattern_into_tracker(void);
void editor_command(int id);

#endif // tracker.h