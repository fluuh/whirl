#include "builtin.h"

#define DEF_BUILTIN(n, r, ...) \
	{.name = n, .ret = r, .args = {__VA_ARGS__, wrl_void}},
const wrl_builtin wrl_builtins[NUM_BUILTINS] = {
	DEF_BUILTIN("print", wrl_void, wrl_string)
};
