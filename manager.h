#ifndef manager_h

# define manager_h

#include "process/process.h"

/**
 * This signal handler, handles the receipt of
 * SIGALRM which tells us when the simulation is over.
 */
void sighandler(int signum);

/**
 * This function's purpose is to set an alarm to
 * itself, so when it's invoked it can properly
 * clean the environment and show simulation results
 */
void set_alarm(void);

/**
 * This function shows the simulation's results to the screen
 */
void sim_sum(void);

/**
 * This function generates the first elements
 * in our population with parameters
 * that the user passed before.
 */
void gen_population(void);

/**
 * This function will be generating a type (A or B)
 * randomly, basing the seed on time(NULL) (which is a random seed
 * based on the current time, to ensure effective randomness).
 * Intended to be used just for the first elements of the population.
 */
char gen_type(void);

/**
 * This function will be generating a random name, from 'A' to 'Z'.
 * if NULL argument is passed. When old_name is passed it will be appending
 * a random char (from 'A' to 'Z') to the old one.
 */
char* gen_name(char* old_name);

/**
 * This function will be generating a random unsigned long
 * number that will be used, if x is 0 then the generated number is at least 2
 * else it's at least x.
 */
unsigned long gen_genome(unsigned long x);

/**
 * gen_child: a function which generates a new child, returns
 * the pinfo of the generated child. if params is specified, then parameters of
 * the child are inherited from them
 */
pinfo_t gen_child(char* o_name, unsigned long xseed);

/**
 * grant_diversity: a function which taken a pinfo_t process grants its diversity
 * from every other individual in the gen_population
 */
void grant_diversity(pinfo_t *member, pinfo_arr_t *community);

/**
 * create_death: a function which generates another process which only purpose is
 * to wake up the manager after birth_death seconds.
 */
void create_death(void);

/**
 * manager's cleaning process.
 */
void mgr_cleanup(void);

void termlistener(pid_t pid);

#endif

