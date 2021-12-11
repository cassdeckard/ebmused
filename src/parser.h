#ifndef _PARSER_H_
#ifndef _WINDOWS_
#include <windows.h>
#endif

#define _PARSER_H_
extern const BYTE code_length[];
void parser_init(struct parser *p, const struct channel_state *c, struct track *subs);
BYTE *next_code(BYTE *p);
BOOL parser_advance(struct parser *p);
int parser_advance_to_next_note(struct parser *p);
int parser_get_pattern_length(struct parser *p);

#endif // _PARSER_H_