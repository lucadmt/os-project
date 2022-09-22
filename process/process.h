#include <sys/types.h>
#ifndef process_h
# define process_h

#define TELL(X,Y,Z) {printf(CYAN("(%s|%c): %s"), X, Y, Z); fflush(stdout);}

typedef struct {
    pid_t pid;
    char type;
    char name[256];
    unsigned long genome;
} pinfo_t;

typedef struct
{
	pinfo_t *array;
	int used;
	int size;
} pinfo_arr_t;

typedef struct mmsgbuf {
    // mandatory, that field will make the receiver understand what we want.
    long msg_type;
    // subject will be the sender of the message.
    pinfo_t subject;
} msgbuf_t;

/**
 * cleanup: a function which cleans up all the resources used by processes.
 */
void cleanup(void);

/**
 * choose_the_one: a function which calculates which one of A processes
 * can grant to B process a high genome.
 */
pinfo_t choose_the_one(pinfo_arr_t *array, pinfo_t me);

/**
 * is_process_eligible: returns wether a B process is eligible for this A
 * process
 */
int is_process_eligible(pinfo_t process, pinfo_t self, pinfo_arr_t *exes);

/**
 * greatest_mcd: a function which returns the greatest number in a set
 */
unsigned long greatest_mcd(pinfo_arr_t *subset);

#endif

