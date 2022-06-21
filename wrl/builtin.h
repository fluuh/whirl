/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef WRL_BUILTIN_H_
#define WRL_BUILTIN_H_

#include "common.h"

/* builtin functions */

#define NUM_BUILTINS 1

typedef struct wrl_builtin_s {
	const char *name;
	wrl_type ret;
	/* terminated by wrl_void */
	wrl_type args[16];
} wrl_builtin;

extern const wrl_builtin wrl_builtins[NUM_BUILTINS];

#endif
