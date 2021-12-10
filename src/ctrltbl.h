#ifndef _CTRLTBL_H_
#ifndef _WINDOWS_
#include <windows.h>
#endif

// ctrltbl.c
#define _CTRLTBL_H_
struct control_desc {
	char *class; short x, y, xsize, ysize; char *title; DWORD id; DWORD style;
};
struct window_template {
	int num, lower, winsize, divy; const struct control_desc *controls;
};
#ifdef CreateWindow
void create_controls(HWND hWnd, struct window_template *t, LPARAM cs);
void move_controls(HWND hWnd, struct window_template *t, LPARAM lParam);
#endif

#endif // _CTRLTBL_H_
