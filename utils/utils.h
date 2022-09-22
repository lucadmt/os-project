#ifndef utils_h

# define utils_h

#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>    // semget
#include <sys/sem.h>    // semget
#include <sys/shm.h>    // shmget
#include <string.h>
#include "../process/process.h" // needed for pinfo_t

#define RESET   "\x1b[0m"
#define RED(X)     "\x1b[31m" X RESET
#define GREEN(X)   "\x1b[32m" X RESET
#define YELLOW(X)  "\x1b[33m" X RESET
#define BLUE(X)    "\x1b[34m" X RESET
#define MAGENTA(X) "\x1b[35m" X RESET
#define CYAN(X)    "\x1b[36m" X RESET
#define ERR(X) RED(X)
#define LOG(X) YELLOW(X)
// #define LOG(X) printf(YELLOW(X));

#define PRINT_ERR fprintf(stderr, ERR("%s: %d. Error #%03d: %s\n"), __FILE__, __LINE__, errno, strerror(errno));

#define WHOAMI fprintf(stderr, CYAN("\nI'm PID no.: %d\nBorn from parent no.: %d\n"), getpid(), getppid());

/**
 * iinput: integer input function with sanitization
 * This function lets the user input an integer.
 * Then it checks that:
 *  - input is integer
 * Outputs: Sanitized input
 */
void iinput(int* ptr, int bufsize);

/**
 * Checks that character's ASCII value is between 0 and 9
 * so, that the character is a digit
 */
int isDigit(char character);


/**
 * clearbuf: a function that skips unused/unread user input.
 * There are times in which, user input is not completely read, in that cases
 * unread input stays into the buffer, this is a problem, because when we ask
 * for input again, the program is going to look into the buffer to see if
 * there is any input, and it finds it (even though we didn't input anything)
 */
void clearbuf(void);

/**
 * count_input: simple function which, gave a string buffer and its size,
 * outputs how many characters the user has effectively inputed.
 * In the use of this function we suppose that the buffer is initialized with
 * emptyfill before using it.
 * (That might cause problems, if not correctly initialized)
 */
int count_input(char* buffer, int size);

/**
 * emptyfill: a function which, gave a string buffer and integer dimensions,
 * initializes each element to an empty char
 */
void emptyfill(char* buffer, int dimensions);

/**
 * get_property: gets a property of the simulation from a file
 */
char* get_property(FILE* stream);

/**
 * print_proc: Prints informations on a process pinfo_t
 */
void print_proc(pinfo_t proc);

/**
 * calc_mcd: a function which calculates the MCD between 2 unsigned longs
 */
unsigned long calc_mcd(unsigned long x, unsigned long y);

/**
 * is_multiple: a function which, gave two numbers, returns wether the first done
 * is multiple of the second.
 */
int is_multiple(unsigned long x, unsigned long y);

/**
 * pinfo_arr_new: a function which allocates space for a new pinfo_arr
 * and returns the pointer to that memory.
 */
pinfo_arr_t* pinfo_arr_new(int arritems);

/**
 * pinfo_arr_new: a function which allocates space for a new pinfo_arr
 * and returns the pointer to that memory.
 */
void pinfo_arr_free(pinfo_arr_t *allocd);

/**
 * contains: a function which gave a member and a subset, if
 * the subset contains the member returns its position int it,
 * -1 otherwise.
 */
int contains(pinfo_t member, pinfo_arr_t *subset);

/**
 * remove: a function which gave a member and a subset, returns the position
 * in the subset, of the removed element, -1 otherwise.
 */
int pinfo_remove(pinfo_t member, pinfo_arr_t *subset);

/**
 * pinfo_arr_cpy: a function which copies elements from x to y 
 * (without overwriting the address) returns 0 if successful, -1 otherwise
 */
int pinfo_arr_cpy(pinfo_arr_t *dest, pinfo_arr_t *src);

/**
 * insert: a function which gave a member and a subset, returns the position in which
 * the element is inserted, -1 otherwise.
 */
int insert(pinfo_t member, pinfo_arr_t *subset);

/**
 * is_empty: a function which gave a pinfo_t element, returns 0 if it's not empty,
 * 1 otherwise.
 */
int is_empty(pinfo_t elem);

/**
 * pinfo_emptyfill: a function which gave a subset, fills it with 0 and '\0' for each
 * corrispondant element. returns -1 if there's no element to fill, 1 otherwise.
 */
int pinfo_emptyfill(pinfo_arr_t *subset);

/**
 * greatest_name: a function which, gave a subset of pinfo_t, returns the position
 * in the array, of the element with the greatest name length, -1 if empty.
 */
pinfo_t greatest_name(pinfo_arr_t *subset);

/**
 * greatest_genome: a function which, gave a subset of pinfo_t, returns the position
 * in the array, of the element with the greatest genome, -1 if empty.
 */
pinfo_t greatest_genome(pinfo_arr_t *subset);

/**
 * searchby_pid: a function which, gave a subset and a pid, searches into the subset
 * a pinfo_t which matches the pid, returns its position in the subset.
 */
int searchby_pid(pinfo_arr_t *subset, pid_t pid);

/**
 * empty: a function which, gave a single object voids it.
 */
void empty(pinfo_t *element);

/**
 * print_arr: utility function to print an entire array of pinfo_t
 */
void print_arr(pinfo_arr_t *arr);

/**
 * getsem: a function which wraps up the operations needed to get a new semaphore
 */
int getsem(int ival);

/**
 * acquire: a function specific to semaphores that wraps up all the operations
 * to acquire the resource protected by the semaphore.
 */
int acquire(int sid);

/**
 * release: a function specific to semaphores that wraps up all the operations
 * to release the resource protected by the semaphore.
 */
int release(int sid);

/**
 * count: a function specific to semaphores that wraps up all the operations
 * to count the resource protected by the semaphore.
 */
int count(int sid);

/**
 * destroy: a function specific to semaphores that wraps up all the operations
 * to remove the resource protected by the semaphore.
 */
int destroy(int sid);

/**
 * get_shmem: a function which wraps up all the operation needed to set up
 * shared memory.
 */
int get_shmem(pinfo_arr_t *subset, int size);

/**
 * rem_shmem: a function which wraps up all the operation needed to remove
 * shared memory by its id.
 */
int rem_shmem(int id);
#endif

