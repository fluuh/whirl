/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef WRL_READER_H_
#define WRL_READER_H_

#include "value.h"

#define READER_MAX_ERRORS 16

typedef struct reader_error_s *reader_error_t;

typedef struct reader_error_s {
	reader_error_t next;
	int row;
	int col;
	const char *str;
} *reader_error_t;

/* whirl -> list */
reader_error_t wrl_read(const char *src, wrl_value_t *dst);

#endif
