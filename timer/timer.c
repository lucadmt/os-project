#include <unistd.h>
#include <signal.h>
#include "../utils/utils.h"

int birth_death;

void timer_sig_handler(int signum)
{
	switch(signum)
	{
		case SIGALRM:
			printf("[timer|%d]Waking parent...\n", getpid());
			kill(getppid(), SIGUSR1);
			alarm(birth_death);
		break;
	}
}

int main(int argc, char* argv[])
{
	birth_death = atoi(argv[1]);
    struct sigaction sa;
    sigset_t  my_mask;
    
    // Starting by sending an alarm to self
    alarm(birth_death);
    
    // Initialize the sigaction
    sa.sa_handler = &timer_sig_handler;
    sa.sa_flags = 0;
    
    sigemptyset(&my_mask); // Do not mask any signal
    sa.sa_mask = my_mask;
    
    sigaction(SIGALRM, &sa, NULL);

    while (1) {
        
    }
}