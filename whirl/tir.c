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
