/* See LICENSE file for copyright and license details. */
#include <sys/types.h>
#include <sys/wait.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LEN(x) (sizeof (x) / sizeof *(x))

static void poweroff(void);
static void reap(void);
static void reboot(void);
static void spawn(char *const []);

static struct {
	int sig;
	void (*handler)(void);
} sigmap[] = {
	{ SIGUSR1, poweroff },
	{ SIGCHLD, reap     },
	{ SIGINT,  reboot   },
};

#include "config.h"

static sigset_t set;

int
main(void)
{
	int sig;
	size_t i;

	if (getpid() != 1)
		return 1;
	chdir("/");
	sigfillset(&set);
	sigprocmask(SIG_BLOCK, &set, NULL);
	spawn(rcinitcmd);
	while (1) {
		sigwait(&set, &sig);
		for (i = 0; i < LEN(sigmap); i++) {
			if (sigmap[i].sig == sig) {
				sigmap[i].handler();
				break;
			}
		}
	}
	/* not reachable */
	return 0;
}

static void
poweroff(void)
{
	spawn(rcpoweroffcmd);
}

static void
reap(void)
{
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
}

static void
reboot(void)
{
	spawn(rcrebootcmd);
}

static void
spawn(char *const argv[])
{
	pid_t pid;

	pid = fork();
	if (pid < 0) {
		perror("fork");
	} else if (pid == 0) {
		sigprocmask(SIG_UNBLOCK, &set, NULL);
		setsid();
		execvp(argv[0], argv);
		perror("execvp");
		_exit(1);
	}
}
