/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

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
	reader_error *first;
	reader_error *last;
} reader_t;

static void report_error(reader_t *p, const char *s)
{
	reader_error *err = wmalloc(sizeof(*err));
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
	   c == '/' || c == '%' ||
	   c == '.' || /* sugar for (get a b) */
	   c == ':' /* sugar for (invoke a b ...) */;
}

static wrl_value *read_expr(reader_t *p);

static wrl_value *read_list(reader_t *p)
{
	nextc(p);
	wrl_list *list = wrl_list_empty();
	while(p->c != ')') {
		if(p->c == 0) {
			wrl_list_free(list);
			return wrl_val_nop();
		}
		wrl_list_append(list, read_expr(p));
	}
	nextc(p);
	return wrl_val_list(list);
}


static wrl_value *read_num(reader_t *p)
{
	
}

static wrl_value *read_sym(reader_t *p)
{
	int start = p->i - 1;
	int len = 0;
	while(is_sym(p->c)) {
		len++;
		nextc(p);
	}
	char *str = wmalloc(len + 1);
	memcpy(str, &p->src[start], len);
	str[len] = 0;
	return wrl_val_sym(str);
}

static wrl_value *read_str(reader_t *p)
{
	nextc(p);
	int start = p->i - 1;
	int len = 0;
	while(p->c != '"') {
		if(p->c == 0) {
			report_error(p, "unexpected eof");
			return wrl_val_nop();
		}
		len++;
		nextc(p);
	}
	nextc(p);
	char *str = wmalloc(len + 1);
	memcpy(str, &p->src[start], len);
	str[len] = 0;
	return wrl_val_str(str);
}

static wrl_value *read_expr(reader_t *p)
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
	if(p->c == '"') {
		/* string */
		return read_str(p);
	}
	/* \0 */
	if(p->c == '\0') {
		return wrl_val_nop();
	}
	/* error */
	report_error(p, "unexpected character");
	return wrl_val_nop();
}

static wrl_value *read_top(reader_t *p)
{
	nextc(p);
	wrl_list *list = wrl_list_empty();
	while(p->c != 0) {
		wrl_value *val = read_expr(p);
		if(val == NULL) {
			/* fatal error */
			return NULL;
		} else if(val->ty == val_nop) {
			nextc(p);
			continue;	
		}
		wrl_list_append(list, val);
	}
	return wrl_val_list(list);
}

reader_error *wrl_read(const char *src, wrl_value **dst)
{
	reader_t *p = wmalloc(sizeof(*p));
	p->src = src;
	p->c = src[0];
	p->i = 0;
	p->col = 1;
	p->row = 1;
	p->first = NULL;
	p->last = NULL;
	wrl_value *val = read_top(p);
	*dst = val;
	return p->first;
}
