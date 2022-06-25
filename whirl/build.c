/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "common.h"
#include "build.h"
#include "reader.h"

wrl_build *wrl_build_create(const char *out_dir)
{
	wrl_build *build = wmalloc(sizeof(*build));
	build->mod = NULL;
	build->out_dir = out_dir;
	return build;
}

static int load_module(const char *dir, wrl_list *mod)
{
	
}

struct package {
	const char *name;
};

/* macros shouldn't return, but idc */
#define val_is(val, t) if(val->ty != t) {return -1;}

static int parse_pkg(struct package *pkg, wrl_list *list)
{
	if(list->len < 2) {
		return -1;
	}
	val_is(list->val[1], val_symbol);
	pkg->name = list->val[1]->str;
	return 0;
}

static int parse_top(struct package *pkg, wrl_list *list)
{
	if(list->len < 1 || list->val[0]->ty != val_symbol) {
		return -1;
	}
	const char *n = list->val[0]->str;
	if(strcmp("package", n)) {
		if(pkg->name != NULL) {
			/* this was probably already loaded */
			return -1;
		}
		parse_pkg(pkg, list);
	}
	return -1;
}

static int load_package(struct package *pkg, wrl_list *list)
{
	/* @inbounds lol */
	for(int i = 0; i < list->len; i++) {
		if(list->val[i]->ty != val_list) {
			return -1;
		}
		if (parse_top(pkg, list->val[i]->list) < 0) {
			return -1;
		}
	}
}

int wrl_load_package(wrl_build *build, const char *src)
{
	wrl_value *list;
	if(wrl_read(src, &list) < 0) {
		return -1;
	}
	struct package *pkg = wmalloc(sizeof(*pkg));
}
