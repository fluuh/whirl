#include <stdio.h>
#include "common.h"
#include "value.h"

static wrl_value_t val_create(int ty)
{
	wrl_value_t val = malloc(sizeof(*val));
	val->ty = ty;
	val->str = NULL;
	return val;
}

wrl_value_t wrl_val_nop(void)
{
	return val_create(val_nop);
}

wrl_value_t wrl_val_list(wrl_list_t list)
{
	wrl_value_t val = val_create(val_list);
	val->list = list;
	return val;
}

wrl_value_t wrl_val_quote(wrl_list_t list)
{
	wrl_value_t val = val_create(val_quote);
	val->list = list;
	return val;
}

wrl_value_t wrl_val_num(double num)
{
	wrl_value_t val = val_create(val_num);
	val->num = num;
	return val;
}

wrl_value_t wrl_val_sym(const char *s)
{
	wrl_value_t val = val_create(val_symbol);
	val->str = s;
	return val;
}

wrl_value_t wrl_val_str(const char *s)
{
	wrl_value_t val = val_create(val_str);
	val->str = s;
	return val;
}

void wrl_val_free(wrl_value_t val)
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

static void print_list(wrl_list_t list, const char *surround)
{
	fputc(surround[0], stdout);
	for(int i = 0; i < list->len; i++) {
		wrl_val_print(list->val[i]);
	}
	fputc(surround[1], stdout);
}

void wrl_val_print(wrl_value_t val)
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
