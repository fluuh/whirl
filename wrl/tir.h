#ifndef WRL_TIR_H_
#define WRL_TIR_H_

#include "common.h"

/* basic types */

/* pointer to a function-owned string.
 * this will be freed with the function. */
typedef const char *tx_str;

/* register id */
typedef int tx_reg;

/* constant id */
typedef int tx_cst;

/* expressions */

#define EXPR(ENUM, NAME, TYPE) TCODE_##ENUM,

typedef enum {
#include "tir.def"
	TX_LAST_UNUSED, /* a way to get the value for NUM_TX_CODES */
} tx_code;

#undef EXPR

#define NUM_TX_CODE ((int) TX_LAST_UNUSED)

/* defined in tir.c */
extern const char *const tx_name[NUM_TX_CODE];
extern const char *const tx_type[NUM_TX_CODE];
extern const unsigned char tx_length[NUM_TX_CODE];

typedef union {
	tx_str str;
	tx_reg reg;
	tx_cst cst;
} tx_union;

typedef struct tx_expr_s {
	tx_code code;
	wrl_type ty;
	tx_union u[];
} tx_expr;


typedef struct tx_expr_list_s tx_expr_list;
struct tx_expr_list_s {
	tx_expr_list *next;
	tx_expr *e;
};

typedef struct tir_func_s tir_func;
struct tir_func_s {
	int n_args;
	tx_expr_list *e;
	int n_c;
	tir_func *c[TIR_MAX_FUNC];
};

typedef struct tir_sym_s {
	enum {
		resolve_use,
		resolve_raw,
	} resolve;
	enum {
		sym_func,
	} ty;
	const char *name;
	union {
		struct {
			int n_args;
		} fn;
	};
} tir_sym;

typedef struct tir_qual_s {
	/* NULL-terminated */
	const char *q[TIR_QUAL_MAX];
} tir_qual;

/* a module declaration */
typedef struct tir_decl_s {
	int cap;
	int len;
	tir_sym **sym;
} tir_decl;

typedef struct tir_module_s {
	tir_qual qual;
	tir_decl decl;
	tir_func *top;
} tir_module;

typedef struct tir_context_s {
	int cap;
	int len;
	tir_module **m;
} tcx;

#endif
