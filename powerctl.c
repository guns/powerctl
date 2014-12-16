#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SYSTEMCTL       "/usr/bin/systemctl"
#define PROGNAME        "powerctl"
#define SUSPEND         "suspend"
#define SYS_SUSPEND     "SUSPEND"
#define REBOOT          "reboot"
#define SYSRQ_REBOOT    "REBOOT"
#define POWEROFF        "poweroff"
#define SLOCK           "slock"
#define SLOCK_SERVICE   "slock.service"

#define SYSRQ_TRIGGER   "/proc/sysrq-trigger"
#define SYS_POWER_STATE "/sys/power/state"

#define MAX_ARGS        0x10

static void die() __attribute__((noreturn));
static void die()
{
	fprintf(stderr, "Usage: %s %s|%s|%s|%s|%s|%s\n",
	        PROGNAME,
	        SUSPEND,
	        SYS_SUSPEND,
	        REBOOT,
	        SYSRQ_REBOOT,
	        POWEROFF,
	        SLOCK);
	exit(1);
}

static int write_string(const char *s, const char *path)
{
	FILE *f = NULL;

	f = fopen(path, "we");
	if (f == NULL) goto fail;
	if (fputs(s, f) == EOF) goto close;
	fclose(f);
	return ferror(f);

close:
	fclose(f);
fail:
	perror(path);
	return -1;
}

int main(int argc, char const **argv)
{
	const char *args[MAX_ARGS + 1] = { NULL };

	if (argc != 2) die();

	if (strcmp(argv[1], SYSRQ_REBOOT) == 0) {
		return !!write_string("b", SYSRQ_TRIGGER);
	} else if (strcmp(argv[1], SYS_SUSPEND) == 0) {
		return !!write_string("mem", SYS_POWER_STATE);
	} else if (strcmp(argv[1], SUSPEND) == 0) {
		args[0] = SYSTEMCTL;
		args[1] = SUSPEND;
		args[2] = "--force";
		args[3] = "--ignore-inhibitors";
	} else if (strcmp(argv[1], REBOOT) == 0) {
		args[0] = SYSTEMCTL;
		args[1] = REBOOT;
		args[2] = "--force";
		args[3] = "--ignore-inhibitors";
	} else if (strcmp(argv[1], POWEROFF) == 0) {
		args[0] = SYSTEMCTL;
		args[1] = POWEROFF;
		args[2] = "--force";
		args[3] = "--ignore-inhibitors";
	} else if (strcmp(argv[1], SLOCK) == 0) {
		args[0] = SYSTEMCTL;
		args[1] = "start";
		args[2] = SLOCK_SERVICE;
	} else {
		die();
	}

	return execv(args[0], (char * const *)args);
}
