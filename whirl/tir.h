/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

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
#define GET_TX_NAME(CODE) (tx_name[(int) CODE])

extern const char *const tx_type[NUM_TX_CODE];
#define GET_TX_TYPE(CODE) (tx_type[(int) CODE])

extern const unsigned char tx_length[NUM_TX_CODE];
#define GET_TX_LENGTH(CODE) (tx_length[(int) CODE])
/* get size of code in bytes */
#define GET_TX_SIZE(CODE) (GET_TX_LENGTH(CODE) * sizeof(tx_union))

typedef struct tx_qual_s {
	/* NULL-terminated */
	const char *q[TIR_QUAL_MAX];
} tx_qual;

/* takes ownership of the strings */
tx_qual *tx_qual_create(const char *q[], int n);
void tx_qual_free(tx_qual *q);

typedef struct tx_expr_s tx_expr;
typedef struct tx_expr_list_s tx_expr_list;

typedef union {
	tx_str str;
	tx_reg reg;
	tx_cst cst;
	tx_qual *qual;
	tx_expr *expr;
	tx_expr_list *xlist;
} tx_union;

struct tx_expr_s {
	tx_code code;
	wrl_type ty;
	tx_union u[];
};

/* allocate an expression */
tx_expr *tx_expr_create(tx_code code);

void tx_expr_free(tx_expr *x);

struct tx_expr_list_s {
	tx_expr_list *next;
	tx_expr *x;
};

tx_expr_list *tx_expr_list_create(tx_expr_list *prev, tx_expr *x);
void tx_expr_list_free(tx_expr_list *list);
void tx_expr_list_destroy(tx_expr_list *list);

typedef struct tir_func_s tir_func;
struct tir_func_s {
	int n_args;
	tx_expr_list *start;
	tx_expr_list *end;
	int n_c;
	tir_func *c[TIR_MAX_FUNC];
};

tir_func *tir_func_create(tir_func *parent, int n_args);
/* free function, but not children */
void tir_func_free(tir_func *fn);
/* free function and it's children */
void tir_func_destroy(tir_func *fn);

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

typedef struct tir_module_s tir_module;

/* a module declaration */
typedef struct tir_decl_s {
	int cap;
	int len;
	tir_sym **sym;
} tir_decl;

tir_decl *tir_decl_create(tir_module *mod, size_t cap);
void tir_decl_free(tir_decl *decl);

typedef struct tir_context_s tcx;

struct tir_module_s {
	tx_qual *qual;
	tir_decl decl;
	tir_func *top;
};

tir_module *tir_module_create(tcx *cx, tx_qual *qual);
/* only frees top */
void tir_module_free(tir_module *mod);
/* frees module */
void tir_module_destroy(tir_module *mod);

struct tir_context_s {
	int cap;
	int len;
	tir_module **m;
};

tcx *tcx_create(void);
void tcx_free(tcx *cx);

#endif
