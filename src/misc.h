#ifndef _MISC_H_
#define _MISC_H_

#ifndef _WINDOWS_
#include <windows.h>
#endif

void enable_menu_items(const BYTE *list, int flags);
#ifdef CreateWindow
void set_up_hdc(HDC hdc);
void reset_hdc(HDC hdc);
#endif
#ifdef EOF
int fgetw(FILE *f);
#endif
BOOL SetDlgItemHex(HWND hwndDlg, int idControl, unsigned int uValue, int size);
int GetDlgItemHex(HWND hwndDlg, int idControl);
int MessageBox2(char *error, char *title, int flags);
// Don't declare parameters, to avoid pointer conversion warnings
// (you can't implicitly convert between foo** and void** because of
//  word-addressed architectures. This is x86 so it's ok)
void *array_insert(/*void **array, int *size, int elemsize, int index*/);
void array_delete(void *array, int *size, int elemsize, int index);

#endif // _MISC_H_