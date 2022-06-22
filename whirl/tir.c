/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "tir.h"

/* the name */

#define EXPR(ENUM, NAME, TYPE) NAME ,

const char *const tx_name[NUM_TX_CODE] = {
#include "tir.def"
};

#undef EXPR

/* the type
 * "s" a pointer to a string
 * "r" a register
 * "c" a constant
 * "f" a pointer to a function
 * "b" a builtin function
 * "q" a name qualifier
 * "e" an expression
 * "E" a list of expressions
 */

#define EXPR(ENUM, NAME, TYPE) TYPE ,

const char *const tx_type[NUM_TX_CODE] = {
#include "tir.def"
};

#undef EXPR

/* the number of operands */

#define EXPR(ENUM, NAME, TYPE) sizeof TYPE - 1 ,

const unsigned char tx_length[NUM_TX_CODE] = {
#include "tir.def"
};

#undef EXPR

/* tx_expr */

tx_expr *tx_expr_create(tx_code code)
{
	tx_expr *x = wmalloc(sizeof(*x) + GET_TX_SIZE(code));
	x->code = code;
	x->ty = wrl_any;
	return x;
}

void tx_expr_destroy(tx_expr *x)
{
	for(int i = 0; i < GET_TX_LENGTH(x->code); i++) {
		/* I'm lucky optimizers exist */
		switch(GET_TX_TYPE(x->code)[i]) {
		case('e'):
			tx_expr_destroy(x->u[i].expr);
			break;
		case('E'):
			tx_expr_list_free(x->u[i].xlist);
		}
	}
	wfree(x);
}

void tx_expr_free(tx_expr *x)
{
	wfree(x);
}

/* tx_expr_list */

tx_expr_list *tx_expr_list_create(tx_expr_list *prev, tx_expr *x)
{
	tx_expr_list *xlist = wmalloc(sizeof(*xlist));
	xlist->x = x;
	xlist->next = NULL;
	if(prev != NULL) {
		prev->next = xlist;
	}
	return xlist;
}

void tx_expr_list_free(tx_expr_list *list)
{
	while(list != NULL) {
		tx_expr_list *next = list->next;
		tx_expr_free(list->x);
		wfree(list);
		list = next;
	}
}

void tx_expr_list_destroy(tx_expr_list *list)
{
	while(list != NULL) {
		tx_expr_list *next = list->next;
		tx_expr_destroy(list->x);
		wfree(list);
		list = next;
	}
}

/* tir_func */

tir_func *tir_func_create(tir_func *parent, int n_args)
{
	tir_func *fn = wmalloc(sizeof(*fn));
	tx_expr *x = tx_expr_create(TCODE_NOP);
	fn->n_args = n_args;
	/* initialize so we don't have to check for NULL later */
	fn->start = tx_expr_list_create(NULL, x);
	fn->end = fn->start;
	return fn;
}

void tir_func_free(tir_func *fn)
{
	tx_expr_list_destroy(fn->start);
	wfree(fn);
}

void tir_func_destroy(tir_func *fn)
{
	tx_expr_list_destroy(fn->start);
	for(int i = 0; i < fn->n_c; i++) {
		tir_func_destroy(fn->c[i]);
	}
	wfree(fn);
}

/* tir_decl */

tir_decl *tir_decl_create(tir_module *mod, size_t cap)
{
	tir_decl *decl = &mod->decl;
	if(cap == 0) {
		cap = TIR_DECL_MAX;
	}
	decl->cap = cap;
	decl->len = 0;
	decl->sym = wmalloc(sizeof(*decl->sym) * cap);
	return decl;
}

void tir_decl_free(tir_decl *decl)
{
	for(int i = 0; i < decl->len; i++) {
		wfree_const(decl->sym[i]->name);
		wfree(decl->sym[i]);
	}
	wfree(decl->sym);
}

/* tir_module */

tir_module *tir_module_create(tx_qual *qual)
{
	tir_module *mod = wmalloc(sizeof(*mod));
	mod->qual = qual;
	tir_decl_create(mod, 0);
	mod->top = NULL;
	return mod;
}

void tir_module_free(tir_module *mod)
{
	tir_func_destroy(mod->top);
	mod->top = NULL;
}

void tir_module_destroy(tir_module *mod)
{
	tx_qual_free(mod->qual);
	tir_decl_free(&mod->decl);
	tir_func_destroy(mod->top);
	wfree(mod);
}
