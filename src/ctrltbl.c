#define WIN32_LEAN_AND_MEAN
#ifndef _WINDOWS_
#include <windows.h>
#endif
#include "ctrltbl.h"
#include "main.h"

void create_controls(HWND hWnd, struct window_template *t, LPARAM cs) {
	int top = 0;
	int width = ((CREATESTRUCT *)cs)->cx;
	int winheight = ((CREATESTRUCT *)cs)->cy;
	t->winsize = MAKELONG(width, winheight);
	int height = t->divy;
	const struct control_desc *c = t->controls;

	for (int num = t->num; num; num--, c++) {
		int x = c->x, y = c->y, xsize = c->xsize, ysize = c->ysize;
		if (num == t->lower) {
			top = t->divy;
			height = winheight - t->divy;
		}
		if (x < 0) x += width;
		if (y < 0) y += height;
		if (xsize <= 0) xsize += width;
		if (ysize <= 0) ysize += height;
		HWND w = CreateWindow(c->class, c->title,
			WS_CHILD | WS_VISIBLE | c->style,
			x, top + y, xsize, ysize,
			hWnd, (HMENU)c->id, hinstance, NULL);
		if (c->class[1] != 'y')
			SendMessage(w, WM_SETFONT, (WPARAM)hfont, 0);
	}
}

void move_controls(HWND hWnd, struct window_template *t, LPARAM lParam) {
	int width = LOWORD(lParam);
	int top, height;
	int i = 0;
	int dir = 1;
	int end = t->num;
	// move controls in reverse order when making the window larger,
	// so that they don't get drawn on top of each other
	if (lParam > t->winsize) {
		i = t->num - 1;
		dir = -1;
		end = -1;
	}
	for (; i != end; i += dir) {
		const struct control_desc *c = &t->controls[i];
		int x = c->x, y = c->y, xsize = c->xsize, ysize = c->ysize;
		if (i < (t->num - t->lower)) {
			top = 0;
			height = t->divy;
		} else {
			top = t->divy;
			height = HIWORD(lParam) - t->divy;
		}
		if (top == 0 && x >= 0 && y >= 0 && xsize > 0 && ysize > 0)
			continue;
		if (x < 0) x += width;
		if (y < 0) y += height;
		if (xsize <= 0) xsize += width;
		if (ysize <= 0) ysize += height;
		MoveWindow(GetDlgItem(hWnd, c->id), x, top + y, xsize, ysize, TRUE);
	}
	t->winsize = lParam;
}
