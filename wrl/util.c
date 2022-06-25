/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>
#include "util.h"

char *read_file(const char *dir, const char *name)
{
	if(strlen(dir) + strlen(name) > 256) {
		return NULL;
	}
	char path[256];
	sprintf(path, "%s/%s", dir, name);
	FILE *f = fopen(path, "r");
	if(f == NULL) {
		return NULL;
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	rewind(f);
	char *str = malloc(fsize + 1);
	fread(str, fsize, 1, f);
	str[fsize] = 0;
	return str;
}
