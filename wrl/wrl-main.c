#include <stdio.h>
#include "common.h"
#include "whirl.h"

#define ERR_STR "\x1b[0;1;31merror:\x1b[0m "
#define HINT_STR "\x1b[0;1;36mhint:\x1b[0m "

#define CMD_ERR 0
#define CMD_INIT 1
#define CMD_BUILD 2
#define CMD_HELP 3
#define CMD_VERSION 4

static void print_version(void)
{
	fputs(WRL_VERSION "\n", stdout);
}

static void print_usage(char *name)
{
	fputs("Usage:\n    ", stdout);
	fputs(name, stdout);
	fputs(" [options..] <cmd> [args..]\n", stdout);
	fputs("Commands:\n"
	"    init        initialize an empty project\n"
	"    build       build project\n"
	"    help        show usage information\n"
	"    version     show compiler version\n", stdout);
	fputs("Options:\n"
	"    -p <dir>    project directory\n", stdout);
}

struct args_s {
	char *name;
	char *pdir; /* project directory */
	int cmd;
	int num_args;
	char **args;
} args;

static int parse_opt(char **argv, int *i)
{
	switch(argv[*i][1]) {
	case('p'):
		args.pdir = argv[++(*i)];
		return 0;
	default:
		fprintf(stderr, ERR_STR "unknown option: '%c'\n", argv[*i][1]);
		return -1;
	}
}

static int select_cmd(char *cmd)
{
	if(strcmp("init", cmd) == 0) {
		args.cmd = CMD_INIT;
		return 0;
	}
	if(strcmp("build", cmd) == 0) {
		args.cmd = CMD_BUILD;
		return 0;
	}
	if(strcmp("help", cmd) == 0) {
		args.cmd = CMD_HELP;
		return 0;
	}
	if(strcmp("version", cmd) == 0) {
		args.cmd = CMD_VERSION;
		return 0;
	}
	fprintf(stderr, ERR_STR "invalid command: \"%s\"\n", cmd);
	args.cmd = CMD_ERR;
	return -1;
}

static int parse_args(char **argv)
{
	for(int i = 0; argv[i] != NULL; i++) {
		if(argv[i][0] == '-') {
			/* option */
			int s = parse_opt(argv, &i);
			if(s < 0) {
				return -1;
			}
			continue;
		}
		if(select_cmd(argv[i]) > 0) {
			return -1;
		}
		if(argv[i + 1] != NULL) {
			fprintf(stderr, ERR_STR "too many arguments\n");
			fprintf(stderr, HINT_STR "options can only appear before commands\n");
		}
		return 0;
	}
	fprintf(stderr, ERR_STR "command not specified\n");
	return -2;
}

/* run cmd */
static char *read_file(const char *dir, const char *name)
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

static int cmd_build(void)
{
	printf("%s\n", read_file(args.pdir, "Whirl.toml"));
	return 0;
}

static int run_cmd(void)
{
	switch(args.cmd) {
	case(CMD_INIT):
		break;
	case(CMD_BUILD):
		return cmd_build();
	case(CMD_HELP):
		print_usage(args.name);
		break;
	case(CMD_VERSION):
		print_version();
		break;
	}
	return 0;
}

int main(int argc, char **argv)
{
	if(argc == 0) {
		fputs("no arguments?\n", stderr);
		return 1;
	}
	args.name = argv[0];
	args.pdir = ".";
	args.num_args = 0;
	args.cmd = CMD_ERR;
	if(parse_args(&argv[1]) < 0) {
		return 1;
	}
	run_cmd();
	return 0;
}
