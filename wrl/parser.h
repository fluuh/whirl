#ifndef WRL_PARSER_H_
#define WRL_PARSER_H_

#include "ir.h"
#include "value.h"

tir_unit wrl_parse(const char *src);
tir_unit wrl_parse_list(wrl_value_t list);

#endif
