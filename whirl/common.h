/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef WRL_COMMON_H_
#define WRL_COMMON_H_

#define WRL_LIST_DEFAULT 128

#define TIR_MAX_FUNC 256
#define TIR_QUAL_MAX 8
#define TIR_DECL_MAX 256
#define TIR_CX_CAP 16

typedef enum {
	wt_void,
	wt_any,
	wt_nil,
	wt_list,
	wt_func,
	wt_table, /* TODO */
	wt_number,
	wt_string,
} wrl_type;

/* used by most files */
#include <stdlib.h>
#include <string.h>

#define wmalloc(size) malloc(size)
#define wrealloc(p, n) realloc(p, n)
#define wfree(ptr) free(ptr)
#define wfree_const(ptr) free((void*) ptr)

#endif
