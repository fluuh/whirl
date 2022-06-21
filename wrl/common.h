#ifndef WRL_COMMON_H_
#define WRL_COMMON_H_

#define WRL_LIST_DEFAULT 128

#define TIR_MAX_FUNC 256

typedef enum {
	wrl_void,
	wrl_any,
	wrl_nil,
	wrl_list,
	wrl_func,
	wrl_table, /* TODO */
	wrl_number,
	wrl_string,
} wrl_type;

/* used by most files */
#include <stdlib.h>
#include <string.h>

#endif
