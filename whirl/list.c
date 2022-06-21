/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "common.h"
#include "value.h"

static void list_resize(wrl_list_t list, size_t newsize)
{
	if(newsize == 0) {
		newsize = WRL_LIST_DEFAULT;
	}
	list->cap = newsize;
	void *new = malloc(sizeof(*list->val) * newsize);
	if(list->val != NULL) {
		memcpy(new, list->val, list->len);
		free(list->val);
	}
	list->val = new;
}

wrl_list_t wrl_list_empty(void)
{
	wrl_list_t list = malloc(sizeof(*list));
	list->len = 0;
	list->val = NULL;
	list_resize(list, 0);	
	return list;
}

void wrl_list_free(wrl_list_t list)
{
	for(int i = 0; i < list->len; i++) {
		wrl_val_free(list->val[i]);
	}
	free(list->val);
	free(list);
}

int wrl_list_append(wrl_list_t list, wrl_value_t val)
{
	if(list->len >= list->cap) {
		list_resize(list, list->cap * 2);
	}
	list->val[list->len++] = val;
	return list->len - 1;
}

wrl_value_t wrl_list_get(wrl_list_t list, int i)
{
	if(i >= list->len) {
		return NULL;
	}
	return list->val[i];
}

int wrl_list_set(wrl_list_t list, int i, wrl_value_t val)
{
	if(i >= list->len) {
		return -1;
	}
	list->val[i] = val;
	return 0;
}
