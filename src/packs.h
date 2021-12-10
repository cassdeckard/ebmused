#ifndef _PACKS_H_
#ifndef _WINDOWS_
#include <windows.h>
#endif

#define _PACKS_H_
extern const DWORD pack_orig_crc[];
void free_pack(struct pack *p);
void load_songpack(int new_pack);
struct block *get_cur_block(void);
void select_block(int block);
void select_block_by_address(int spc_addr);
struct block *save_cur_song_to_pack(void);
int calc_pack_size(struct pack *p);
void new_block(struct block *b);
void delete_block(int block);
void move_block(int to);
BOOL save_pack(int pack);

#endif //_PACKS_H_