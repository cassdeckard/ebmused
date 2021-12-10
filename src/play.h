#ifndef _PLAY_H_
#ifndef _WINDOWS_
#include <windows.h>
#endif

#define _PLAY_H_
extern BYTE spc[65536];
extern int inst_base;
void set_inst(struct song_state *st, struct channel_state *c, int inst);
void calc_freq(struct channel_state *c, int note16);
void load_pattern(void);
BOOL do_cycle_no_sound(struct song_state *st);
BOOL do_timer(void);
void initialize_state(void);

#endif // _PLAY_H_
