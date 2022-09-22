#ifndef init_h

# define init_h

/**
 * Variables:
 */
extern int init_people; // Number of processes in the simulation
extern int sim_time;    // Time the simulation will be executing
extern int genes;       // Max number of genes that a process should have
extern int birth_death; // Decides for how much time the process should live

/**
 * Initializer Method:
 * There are multiple ways a program can be initializated
 * We choose to apply all of them, so the program should
 * understand in which situation we are, there are 3 scenarios
 * accordingly:
 * - Config file: Parameters are passed in the form of a file path
 * - Command line: Parameters are passed within program invocation, in this
 *   case, user inputs are also sanitized.
 * - Standard input: If none of the above is "understood" by the program
 *                   stdin is the default specifier.
 */
void init(int param_count, char* params);

/**
 * Default simulation initializer, triggered
 * when the user didn't pass parameters in one of these ways:
 * - config file
 * - command line
 */
void sim_init(void);

/**
 * sim_from_file: alternative simulation initializer based on file input
 */
void sim_from_file(char* path);

#endif
