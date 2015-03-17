/* See LICENSE file for copyright and license details. */
#define _POSIX_SOURCE
#include <sys/types.h>
#include <sys/wait.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LEN(x) (sizeof (x) / sizeof *(x))

static void sigexit(void);
static void sigreap(void);
static void spawn(char *const []);

static struct {
	int sig;
	void (*handler)(void);
} sigmap[] = {
	{ SIGTERM, sigexit     },
	{ SIGINT , sigexit     },
	{ SIGCHLD, sigreap     },
};

static sigset_t set;

int
main(int argc, char **argv)
{
	int sig;
	size_t i;
	(void) argc;

	chdir("/");
	sigfillset(&set);
	sigprocmask(SIG_BLOCK, &set, NULL);
	spawn(argv + 1);

	if (getpid() != 1)
		return 1;

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
sigexit(void)
{
	_exit(0);
}

static void
sigreap(void)
{
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
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
