/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "common.h"
#include "value.h"

static void list_resize(wrl_list *list, size_t newsize)
{
	if(newsize == 0) {
		newsize = WRL_LIST_DEFAULT;
	}
	list->cap = newsize;
	list->val = wrealloc(list->val, newsize);
}

wrl_list *wrl_list_empty(void)
{
	wrl_list *list = wmalloc(sizeof(*list));
	list->len = 0;
	list->val = NULL;
	list_resize(list, 0);	
	return list;
}

void wrl_list_free(wrl_list *list)
{
	for(int i = 0; i < list->len; i++) {
		wrl_val_free(list->val[i]);
	}
	free(list->val);
	free(list);
}

int wrl_list_append(wrl_list *list, wrl_value *val)
{
	if(list->len >= list->cap) {
		list_resize(list, list->cap * 2);
	}
	list->val[list->len++] = val;
	return list->len - 1;
}

wrl_value *wrl_list_get(wrl_list *list, int i)
{
	if(i >= list->len) {
		return NULL;
	}
	return list->val[i];
}

int wrl_list_set(wrl_list *list, int i, wrl_value *val)
{
	if(i >= list->len) {
		return -1;
	}
	list->val[i] = val;
	return 0;
}
