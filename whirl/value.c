/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>
#include "common.h"
#include "value.h"

static wrl_value *val_create(int ty)
{
	wrl_value *val = wmalloc(sizeof(*val));
	val->ty = ty;
	val->str = NULL;
	return val;
}

wrl_value *wrl_val_nop(void)
{
	return val_create(val_nop);
}

wrl_value *wrl_val_list(wrl_list *list)
{
	wrl_value *val = val_create(val_list);
	val->list = list;
	return val;
}

wrl_value *wrl_val_quote(wrl_list *list)
{
	wrl_value *val = val_create(val_quote);
	val->list = list;
	return val;
}

wrl_value *wrl_val_num(double num)
{
	wrl_value *val = val_create(val_num);
	val->num = num;
	return val;
}

wrl_value *wrl_val_sym(const char *s)
{
	wrl_value *val = val_create(val_symbol);
	val->str = s;
	return val;
}

wrl_value *wrl_val_str(const char *s)
{
	wrl_value *val = val_create(val_str);
	val->str = s;
	return val;
}

void wrl_val_free(wrl_value *val)
{
	switch(val->ty) {
	case(val_list):
	case(val_quote):
		wrl_list_free(val->list);
		break;
	case(val_symbol):
	case(val_str):
		free((void*)val->str);
		break;
	}
	free(val);
}

static void print_list(wrl_list *list, const char *surround)
{
	fputc(surround[0], stdout);
	for(int i = 0; i < list->len; i++) {
		wrl_val_print(list->val[i]);
	}
	fputc(surround[1], stdout);
}

void wrl_val_print(wrl_value *val)
{
	switch(val->ty) {
	case(val_nop):
		fputs("(nop) ", stdout);
		break;
	case(val_list):
		print_list(val->list, "()");
		fputc(' ', stdout);
		break;
	case(val_quote):
		print_list(val->list, "[]");
		fputc(' ', stdout);
		break;
	case(val_num):
		printf("%f ", val->num);
		break;
	case(val_str):
		printf("\"%s\" ", val->str);
		break;
	case(val_symbol):
		printf("%s ", val->str);
	}
}
