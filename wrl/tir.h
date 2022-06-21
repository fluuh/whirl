#ifndef WRL_TIR_H_
#define WRL_TIR_H_

#include "common.h"

/* basic types */

/* pointer to a function-owned string.
 * this will be freed with the function. */
typedef const char *tx_str;

/* register id */
typedef int tx_reg;

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
	const char *name;
} tir_sym;

typedef struct tir_unit_decl_s {
	int num_sym;
	tir_sym **sym;
} tir_unit_decl;

typedef struct tir_unit_s {
	tir_unit_decl *decl;
} tir_unit;

#endif
