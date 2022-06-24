/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "builtin.h"

#define DEF_BUILTIN(n, r, ...) \
	{.name = n, .ret = r, .args = {__VA_ARGS__, wt_void}},
const wrl_builtin wrl_builtins[NUM_BUILTINS] = {
	DEF_BUILTIN("print", wt_void, wt_string)
};
