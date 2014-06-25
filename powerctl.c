#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define MAX_ARGS      0x10

#define PROGNAME      "powerctl"
#define SUSPEND       "suspend"
#define REBOOT        "reboot"
#define SYSRQ_REBOOT  "REBOOT"
#define POWEROFF      "poweroff"

#define SYSRQ_MASK    "/proc/sys/kernel/sysrq"
#define SYSRQ_TRIGGER "/proc/sysrq-trigger"

static void die() __attribute__((noreturn));
static void die()
{
	fprintf(stderr, "Usage: %s %s|%s|%s|%s\n",
	        PROGNAME,
	        SUSPEND,
	        POWEROFF,
	        REBOOT,
	        SYSRQ_REBOOT);
	exit(1);
}

/* Documentation/sysrq.txt:
 *
 * Here is the list of possible values in /proc/sys/kernel/sysrq:
 *    0 - disable sysrq completely
 *    1 - enable all functions of sysrq
 *   >1 - bitmask of allowed sysrq functions (see below for detailed function
 *        description):
 *           2 =   0x2 - enable control of console logging level
 *           4 =   0x4 - enable control of keyboard (SAK, unraw)
 *           8 =   0x8 - enable debugging dumps of processes etc.
 *          16 =  0x10 - enable sync command
 *          32 =  0x20 - enable remount read-only
 *          64 =  0x40 - enable signalling of processes (term, kill, oom-kill)
 *         128 =  0x80 - allow reboot/poweroff
 *         256 = 0x100 - allow nicing of all RT tasks
 *
 * You can set the value in the file by the following command:
 *     echo "number" >/proc/sys/kernel/sysrq
 */
static int sysrq_set(unsigned short flags)
{
	FILE *f = NULL;
	unsigned short cflags = 0;

	f = fopen(SYSRQ_MASK, "w+e");
	if (f == NULL) goto fail;
	if (fscanf(f, "%hu", &cflags) == EOF) goto close;
	if (!(flags & 0x1)) {
		rewind(f);
		fprintf(f, "%d", flags | cflags);
	}
	fclose(f);
	return ferror(f);
close:
	fclose(f);
fail:
	perror(SYSRQ_MASK);
	return -1;
}

static int sysrq_cmd(char c)
{
	FILE *f = NULL;

	f = fopen(SYSRQ_TRIGGER, "we");
	if (f == NULL) {
		perror(SYSRQ_TRIGGER);
		return -1;
	}

	fputc(c, f);
	fclose(f);
	return ferror(f);
}

int main(int argc, char const **argv)
{
	const char *args[MAX_ARGS + 1] = { NULL };
	int i = 0;

	if (argc != 2) die();

	args[i++] = "/usr/bin/systemctl";

	if (strcmp(argv[1], SYSRQ_REBOOT) == 0) {
		sysrq_set(0x80);
		return !!sysrq_cmd('b');
	} else if (strcmp(argv[1], SUSPEND) == 0) {
		args[i++] = SUSPEND;
		args[i++] = "--force";
		args[i++] = "--ignore-inhibitors";
	} else if (strcmp(argv[1], REBOOT) == 0) {
		args[i++] = REBOOT;
		args[i++] = "--force";
		args[i++] = "--ignore-inhibitors";
	} else if (strcmp(argv[1], POWEROFF) == 0) {
		args[i++] = POWEROFF;
		args[i++] = "--force";
		args[i++] = "--ignore-inhibitors";
	} else {
		die();
	}

	if (i > MAX_ARGS) die();

	return execv(args[0], (char * const *)args);
}
