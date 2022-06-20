#include <stdbool.h>
#include <ctype.h>
#include "common.h"
#include "reader.h"

typedef struct reader_s {
	const char *src;
	char c;
	int i;
	int col;
	int row;
	int num_errors;
	reader_error_t first;
	reader_error_t last;
} reader_t;

static void report_error(reader_t *p, const char *s)
{
	reader_error_t err = malloc(sizeof(*err));
	err->col = p->col;
	err->row = p->row;
	err->str = s;
	if(p->first == NULL) {
		p->first = err;
	}
	if(p->last != NULL) {
		p->last->next = err;
	}
	p->last = err;
	p->num_errors++;
}

static int nextc(reader_t *p) {
	if(p->c == 0) {
		return 0;
	}
	p->c = p->src[p->i++];
	if(p->c == '\n') {
		p->col = 1;
		p->row++;
	} else {
		p->col++;
	}
	return p->c;
}

static int is_whitespace(char c) {
	switch(c) {
	case(' '):
	case('\t'):
	case('\n'):
		return 1;
	default:
		return 0;
	}
}

static int is_sym(char c) {
	return isalnum(c) ||
	   c == '-' || c == '_' ||
	   c == '?' || c == '+' ||
	   c == '-' || c == '*' ||
	   c == '/' || c == '%';
}

static wrl_value_t read_list(reader_t *p)
{
	
}


static wrl_value_t read_num(reader_t *p)
{
	
}

static wrl_value_t read_sym(reader_t *p)
{
	
}

static wrl_value_t read_expr(reader_t *p)
{
	while(is_whitespace(p->c)) {
		nextc(p);
	}
	if(p->c == '(') {
		/* list */
		return read_list(p);
	}
	if(isdigit(p->c)) {
		/* number */
		return read_num(p);
	}
	if(is_sym(p->c)) {
		/* identifier */
		return read_sym(p);
	}
	/* error */
	report_error(p, "unexpected character");
	return wrl_val_nop();
}

static wrl_value_t read_top(reader_t *p)
{
	wrl_list_t list = wrl_list_empty();
	while(p->c != 0) {
		wrl_value_t val = read_expr(p);
		if(val == NULL) {
			/* fatal error */
			return NULL;
		}
		wrl_list_append(list, val);
	}
	return wrl_val_list(list);
}

reader_error_t wrl_read(const char *src, wrl_value_t *dst)
{
	reader_t *p = malloc(sizeof(*p));
	p->src = src;
	p->c = src[0];
	p->i = 0;
	p->col = 1;
	p->row = 1;
	p->first = NULL;
	p->last = NULL;
	wrl_value_t val = read_top(p);
	if(val != NULL) {
		*dst = val;
	}
	return p->first;
}
