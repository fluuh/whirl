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
 * "f" a function identifier
 * "b" a builtin function
 * "q" a name qualifier
 * "e" an expression
 * "E" a list of expressions
 * "u" an upvalue identifier
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

tir_func *tir_func_create(tir_func *parent, int n_args, int n_upval)
{
	tir_func *fn = wmalloc(sizeof(*fn));
	tx_expr *x = tx_expr_create(TCODE_NOP);
	fn->n_upval = n_upval;
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

tir_decl *tir_decl_create(tir_unit *mod, size_t cap)
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

/* tir_unit */

static int unit_insert(tcx *cx, tir_unit *unit, tx_qual *qual)
{
	tir_tree **t;
	tir_tree *tree;
	int len;
	int pos = 0;
	/* get the length */
	for(len = 0; qual->q[len] != NULL; len++);
	/* find where to insert unit */
	for(t = &cx->tree; *t != NULL && pos < len; ) {
		if(strcmp((*t)->name, qual->q[pos]) == 0) {
			if(pos == len - 1) {
				/* unit name already exists */
				tree = *t;
				goto insert;
			}
			t = &(*t)->child;
			pos++;
		} else {
			t = &(*t)->next;
		}
	}
	/* create new nodes */
	tree = malloc(sizeof(*tree));
	tree->name = qual->q[pos];
	tree->next = NULL;
	tree->child = NULL;
	(*t)->next = tree;
	for(;pos < len; pos++) {
		t = &tree->child;
		tree = malloc(sizeof(*tree));
		tree->next = NULL;
		tree->child = NULL;
		tree->name = qual->q[pos];
		*t = tree;
	}
insert:
	tree->m = unit;
	return 0;
}

tir_unit *tir_unit_create(tcx *cx, tx_qual *qual)
{
	tir_unit *unit = wmalloc(sizeof(*unit));
	tir_decl_create(unit, 0);
	unit->top = NULL;
	/* add unit to context */
	if(cx->len >= cx->cap) {
		cx->cap *= 2;
		cx->m = wrealloc(cx->m, cx->cap);
	}
	cx->m[cx->len++] = unit;
	unit_insert(cx, unit, qual);
	return unit;
}

void tir_unit_free(tir_unit *mod)
{
	tir_func_destroy(mod->top);
	mod->top = NULL;
}

void tir_unit_destroy(tir_unit *mod)
{
	tir_decl_free(&mod->decl);
	tir_func_destroy(mod->top);
	wfree(mod);
}

/* tcx */

tcx *tcx_create(void)
{
	tcx *cx = wmalloc(sizeof(*cx));
	cx->cap = TIR_CX_CAP;
	cx->len = 0;
	cx->m = wmalloc(sizeof(*cx->m) * cx->cap);
	return cx;
}

void tcx_free(tcx *cx)
{
	for(int i = 0; i < cx->len; i++) {
		tir_unit_destroy(cx->m[i]);
	}
	wfree(cx->m);
	wfree(cx);
}
