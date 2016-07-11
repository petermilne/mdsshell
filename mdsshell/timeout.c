/* timeout handling */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

#include <setjmp.h>

static jmp_buf* S_env;

static void alarm_handler(int signum) {
        //printf("alarm");
        fprintf(stderr, "ERROR:TIMEOUT\n");
	if (S_env){
		longjmp(*S_env, 1);
	}
}


static void install_handlers(void) {
	static int installed = 0;

	if (!installed){
		struct sigaction sa;
		memset(&sa, 0, sizeof(sa));
		sa.sa_handler = alarm_handler;
		if (sigaction(SIGALRM, &sa, NULL)) perror ("sigaction");

		installed = 1;
	}
}

void set_timeout(int timeout_secs, jmp_buf* env)
{
	alarm(0);
	S_env = env;
	
	if (timeout_secs){
		install_handlers();
		alarm(timeout_secs);
	}
}

