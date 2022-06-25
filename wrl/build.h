/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef WRL_BUILD_H_
#define WRL_BUILD_H_

typedef struct wrl_module_s wrl_module;
struct wrl_module_s {
	const char *name;
	wrl_module *next;
	int num_src;
	const char **src;
	const char *out; /* output directory relative to default */
	/* the next unit to load */
	int load;
};

typedef struct wrl_build_s {
	wrl_module *mod;
	const char *out_dir;
} wrl_build;

wrl_build *wrl_build_create(const char *out_dir);
int wrl_build_build(wrl_build *build);
/* load and parse a package and it's dependencies */
int wrl_load_package(wrl_build *build, const char *src);

#endif
