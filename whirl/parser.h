/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef WRL_PARSER_H_
#define WRL_PARSER_H_

#include "ir.h"
#include "value.h"

tir_unit wrl_parse(const char *src);
tir_unit wrl_parse_list(wrl_value_t list);

#endif
