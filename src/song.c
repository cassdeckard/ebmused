#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "play.h"
#include "parser.h"
#include "main.h"

static char errbuf[60];
char *decomp_error;

static char *internal_validate_track(BYTE *data, int size, BOOL is_sub) {
	for (int pos = 0; pos < size; ) {
		int byte = data[pos];
		int next = pos + 1;

		if (byte < 0x80) {
			if (byte == 0) return "Track can not contain [00]";
			if (next != size && data[next] < 0x80) next++;
			if (next == size) return "Track can not end with note-length code";
		} else if (byte >= 0xE0) {
			if (byte == 0xFF) return "Invalid code [FF]";
			next += code_length[byte - 0xE0];
			if (next > size) {
				char *p = strcpy(errbuf, "Incomplete code: [") + 18;
				for (; pos < size; pos++)
					p += sprintf(p, "%02X ", data[pos]);
				for (; pos < next; pos++)
					p += sprintf(p, "?? ");
				p[-1] = ']';
				return errbuf;
			}

			if (byte == 0xEF) {
				if (is_sub) return "Can't call sub from within a sub";
				int sub = *(WORD *)&data[pos+1];
				if (sub >= cur_song.subs) {
					sprintf(errbuf, "Subroutine %d not present", sub);
					return errbuf;
				}
				if (data[pos+3] == 0) return "Subroutine loop count can not be 0";
			}
		}

		pos = next;
	}
	return NULL;
}

BOOL validate_track(BYTE *data, int size, BOOL is_sub) {
	char *err = internal_validate_track(data, size, is_sub);
	if (err) {
		MessageBox2(err, NULL, 48/*MB_ICONEXCLAMATION*/);
		return FALSE;
	}
	return TRUE;
}

int compile_song(struct song *s) {
	int i;

	// Put order
	WORD *wout = (WORD *)&spc[s->address];
	int first_pat = s->address + s->order_length*2 + (s->repeat ? 6 : 2);
	for (i = 0; i < s->order_length; i++)
		*wout++ = first_pat + (s->order[i] << 4);
	if (s->repeat) {
		*wout++ = s->repeat;
		*wout++ = s->address + s->repeat_pos*2;
	}
	*wout++ = 0;

	// Put patterns and tracks
	BYTE *tracks_start = &spc[first_pat + (s->patterns << 4)];
	BYTE *tout = tracks_start;
	for (i = 0; i < s->patterns; i++) {
		int first = 1;
		for (int ch = 0; ch < 8; ch++) {
			struct track *t = &s->pattern[i][ch];
			if (t->track == NULL) {
				*wout++ = 0;
			} else {
				*wout++ = tout - spc;
				// Only the first track in a pattern is 0 terminated
				int size = t->size + first;
				memcpy(tout, t->track, size);
				tout += size;
				first = 0;
			}
		}
	}
	// There's another 0 before subs start
	*tout++ = 0;
	BYTE *tracks_end = tout;

	// Convert subroutine numbers into addresses, and append the subs as
	// they are used. This is consistent with the way the original songs are:
	// subs are always in order of first use, and there are no unused subs.
	WORD *sub_table = calloc(sizeof(WORD), s->subs);
	for (BYTE *pos = tracks_start; pos < tracks_end; pos = next_code(pos)) {
		if (*pos == 0xEF) {
			int sub = *(WORD *)(pos + 1);
			if (sub >= s->subs) abort(); // can't happen

			if (sub_table[sub] == 0) {
				struct track *t = &s->sub[sub];
				sub_table[sub] = tout - spc;
				memcpy(tout, t->track, t->size + 1);
				tout += t->size + 1;
			}
			*(WORD *)(pos + 1) = sub_table[sub];
		}
	}
	free(sub_table);

	return (tout - spc) - s->address;
}

int decompile_song(struct song *s, int start_addr, int end_addr) {
	char *error = errbuf;
	s->address = start_addr;
	s->changed = FALSE;

	// Get order length and repeat info (at this point, we don't know how
	// many patterns there are, so the pattern pointers aren't validated yet)
	WORD *wp = (WORD *)&spc[start_addr];
	while (*wp >= 0x100) wp++;
	s->order_length = wp - (WORD *)&spc[start_addr];
	if (s->order_length == 0) {
		error = "Order length is 0";
		goto error1;
	}
	s->repeat = *wp++;
	if (s->repeat == 0) {
		s->repeat_pos = 0;
	} else {
		int repeat_off = *wp++ - start_addr;
		if (repeat_off & 1 || repeat_off < 0 || repeat_off >= s->order_length*2) {
			sprintf(errbuf, "Bad repeat pointer: %x", repeat_off + start_addr);
			goto error1;
		}
		if (*wp++ != 0) {
			error = "Repeat not followed by end of song";
			goto error1;
		}
		s->repeat_pos = repeat_off >> 1;
	}

	int first_pattern = (BYTE *)wp - spc;

	int tracks_start;
	// locate first track, determine number of patterns
	while (((BYTE *)wp)+1 < &spc[end_addr] && *wp == 0) wp++;
	if (((BYTE *)wp)+1 >= &spc[end_addr]) {
		// no tracks in the song
		tracks_start = end_addr - 1;
	} else {
		tracks_start = *wp;
	}

	int pat_bytes = tracks_start - first_pattern;
	if (pat_bytes <= 0 || pat_bytes & 15) {
		sprintf(errbuf, "Bad first track pointer: %x", tracks_start);
		goto error1;
	}

	int tracks_end;
	if (((BYTE *)wp)+1 >= &spc[end_addr]) {
		// no tracks in the song
		tracks_end = end_addr - 1;
	} else {
		// find the last track
		int tp, tpp = tracks_start;
		while ((tp = *(WORD *)&spc[tpp -= 2]) == 0);

		if (tp < tracks_start || tp >= end_addr) {
			sprintf(errbuf, "Bad last track pointer: %x", tp);
			goto error1;
		}


		// is the last track the first one in its pattern?
		BOOL first = TRUE;
		int chan = (tpp - first_pattern) >> 1 & 7;
		for (; chan; chan--)
			first &= *(WORD *)&spc[tpp -= 2] == 0;

		BYTE *end = &spc[tp];
		while (*end) end = next_code(end);
		end += first;
		tracks_end = end - spc;
	}

	if (spc[tracks_end] != 0) {
		sprintf(errbuf, "No ending 0 byte after tracks (%02X)", spc[tracks_end]);
		goto error1;
	}

	// Now the number of patterns is known, so go back and get the order
	s->order = malloc(sizeof(int) * s->order_length);
	wp = (WORD *)&spc[start_addr];
	for (int i = 0; i < s->order_length; i++) {
		int pat = *wp++ - first_pattern;
		if (pat < 0 || pat >= pat_bytes || pat & 15) {
			sprintf(errbuf, "Bad pattern pointer: %x", pat + first_pattern);
			goto error2;
		}
		s->order[i] = pat >> 4;
	}

	int next_sub = tracks_end + 1;
	WORD *sub_table = NULL;
	s->patterns = pat_bytes >> 4;
	s->pattern = calloc(sizeof(*s->pattern), s->patterns);
	s->subs = 0;
	s->sub = NULL;

	wp = (WORD *)&spc[first_pattern];
	BOOL first = 0;
	for (int trk = 0; trk < s->patterns * 8; trk++) {
		struct track *t = &s->pattern[0][0] + trk;
		first |= ((trk & 7) == 0);
		int start = *wp++;
		if (start == 0) continue;
		if (start < tracks_start || start >= tracks_end) {
			sprintf(errbuf, "Bad track pointer: %x", start);
			goto error3;
		}

		int next = 0;
		WORD *nextp = wp;
		while (nextp < (WORD *)&spc[tracks_start] && (next = *nextp) == 0)
			nextp++;
		if (next == 0) next = tracks_end; // last track

		if (next <= start) {
			sprintf(errbuf, "Tracks out of order (%04X, %04X)", start, next);
			goto error3;
		}

		if (first && spc[--next] != 0) {
			sprintf(errbuf, "Track %d.%d not zero terminated", trk >> 3, trk & 7);
			goto error3;
		}

		t->size = next - start;
		t->track = memcpy(malloc(t->size + 1), &spc[start], t->size);
		t->track[t->size] = 0;

		for (BYTE *p = t->track; p < t->track + t->size; p = next_code(p)) {
			if (*p != 0xEF) continue;
			int sub_ptr = *(WORD *)(p + 1);
			int i;
			if (sub_ptr == next_sub) {
				i = s->subs++;

				sub_table = realloc(sub_table, sizeof(WORD) * s->subs);
				sub_table[i] = sub_ptr;

				s->sub = realloc(s->sub, sizeof(struct track) * s->subs);
				struct track *st = &s->sub[i];

				BYTE *substart = &spc[sub_ptr];
				BYTE *subend = substart;
				while (*subend != 0) subend = next_code(subend);
				st->size = subend - substart;
				st->track = memcpy(malloc(st->size + 1), substart, st->size + 1);
				char *e = internal_validate_track(st->track, st->size, TRUE);
				if (e) {
					error = e;
					goto error3;
				}

				next_sub = (subend + 1) - spc;
			} else {
				for (i = 0; i < s->subs && sub_table[i] != sub_ptr; i++);
				if (i == s->subs) {
					sprintf(error, "Unexpected subroutine call %04X (next=%04X)", sub_ptr, next_sub);
					goto error3;
				}
			}
			*(WORD *)(p + 1) = i;
		}
		char *e = internal_validate_track(t->track, t->size, FALSE);
		if (e) {
			error = e;
			goto error3;
		}

		first = FALSE;
	}
	free(sub_table);

	return next_sub;

error3:
	free(sub_table);
	for (int trk = 0; trk < s->patterns * 8; trk++)
		free(s->pattern[0][trk].track);
	for (int trk = 0; trk < s->subs; trk++)
		free(s->sub[trk].track);
	free(s->sub);
	free(s->pattern);
error2:
	free(s->order);
error1:
	s->order_length = 0;
	decomp_error = error;
	printf("Can't decompile: %s\n", error);
	return 0;
}

void free_song(struct song *s) {
	int pat, ch, sub;
	if (!s->order_length) return;
	s->changed = FALSE;
	free(s->order);
	for (pat = 0; pat < s->patterns; pat++)
		for (ch = 0; ch < 8; ch++)
			free(s->pattern[pat][ch].track);
	free(s->pattern);
	for (sub = 0; sub < s->subs; sub++)
		free(s->sub[sub].track);
	free(s->sub);
	s->order_length = 0;
}
