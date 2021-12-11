#ifndef _WINDOWS_
#include <windows.h>
#endif
#include "structs.h"
#include "misc.h"

void parser_init(struct parser *p, const struct channel_state *c, struct track *subs) {
	p->ptr = c->ptr;
	p->sub_start = c->sub_start;
	p->sub_ret = c->sub_ret;
	p->sub_count = c->sub_count;
	p->note_len = c->note_len;
	p->subs = subs;
}

BOOL parser_advance(struct parser *p) {
	int chr = *p->ptr;
	if (chr == 0) {
		if (p->sub_count == 0) return FALSE;
		p->ptr = --p->sub_count ? p->subs[p->sub_start].track : p->sub_ret;
	} else if (chr == 0xEF) {
		p->sub_ret = p->ptr + 4;
		p->sub_start = *(WORD *)&p->ptr[1];
		p->sub_count = p->ptr[3];
		p->ptr = p->subs[p->sub_start].track;
	} else {
		if (chr < 0x80)
			p->note_len = chr;
		p->ptr = next_code(p->ptr);
	}
	return TRUE;
}

BYTE parser_advance_to_next_note(struct parser *p) {
	do {
		if (*(p->ptr) >= 0x80 && *(p->ptr) < 0xE0)
			break;
	} while (parser_advance(p));
	return *(p->ptr);
}

int parser_get_pattern_length(struct parser *p) {
	int result = 0;
	do {
		if (*(p->ptr) >= 0x80 && *(p->ptr) < 0xE0)
			result += p->note_len;
	} while (parser_advance(p));
	return result;
}