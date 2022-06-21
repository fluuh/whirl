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
