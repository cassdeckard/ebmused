#ifndef _SOUND_H_
#ifndef _WINDOWS_
#include <windows.h>
#endif

#define _SOUND_H_
// sound.c
extern int mixrate;
extern int chmask;
extern BOOL song_playing;
extern int timer_speed;
int sound_init(void);
void winmm_message(unsigned int uMsg);

#endif //_SOUND_H_