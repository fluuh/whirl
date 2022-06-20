#ifndef WRL_VALUE_H_
#define WRL_VALUE_H_

typedef struct wrl_value_s *wrl_value_t;
typedef struct wrl_list_s *wrl_list_t;

struct wrl_value_s {
	enum {
		val_nop,
		val_list,
		val_quote, /* not really a type but relevant for ast */
		val_num,
		val_symbol,
		val_str,
	} ty;
	union {
		wrl_list_t list; /* or quote */
		const char *str;
		double num;
	};
};

wrl_value_t wrl_val_nop(void);
wrl_value_t wrl_val_list(wrl_list_t list);
wrl_value_t wrl_val_quote(wrl_list_t list);
wrl_value_t wrl_val_num(double num);
/* won't clone s */
wrl_value_t wrl_val_sym(const char *s);
wrl_value_t wrl_val_str(const char *s);
void wrl_val_free(wrl_value_t val);

struct wrl_list_s {
	int cap;
	int len;
	wrl_value_t *val;
};

/* create a new empty list */
wrl_list_t wrl_list_empty(void);

/* free a list and it's content */
void wrl_list_free(wrl_list_t list);

/* append a value to a list
 * returns a negative number on failure
 * or the index on success */
int wrl_list_append(wrl_list_t list, wrl_value_t val);

/* get a value from a list
 * returns NULL on failure
 * or a value on success */
wrl_value_t wrl_list_get(wrl_list_t list, int i);

/* set an existing index in a list
 * returns a negative number on failure
 * or 0 on success 
 */
int wrl_list_set(wrl_list_t list, int i, wrl_value_t val);

#endif
