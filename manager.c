#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "init/init.h"
#include "utils/utils.h"
#include "process/process.h" // needed for pinfo
#include "manager.h"

#define VERBOSE

int sync_start, shmem_id, a_arr_id, dims_shm_id, child_queue, timer, *lengths;
pinfo_t pair[2], new_child, *a_arr;
pinfo_arr_t *stats, *a_stats, *childs;

int main(int argc, char *argv[])
{
    int i = 0;
    
    srand(time(NULL)); // Grant true randomness
    printf("Simulation started. I'm: %d\n", getpid());
    sim_init();
    
    /*
     * A little note.
     * 2 processes die when they want to generate 2 childs,
     * that means the population doesn't grow at all.
     * each birth_death seconds a process dies.
     * which in the best case (short time) we have more than
     * enough space allocating with init_people dimension,
     * in the worst case (very long time) probably 2 processes
     * will be willing to marry each other, which basically leaves
     * the population unaltered.
     *
     * Conclusions: we should be good allocating arrays of init_people
     *              elements within.
     */
    
    stats = pinfo_arr_new(init_people);
    childs = pinfo_arr_new(init_people);
    
    if (!stats || !childs) return -1;
    
<<<<<<< HEAD
    if(!childs)
    {
        perror(ERR("\nERROR #1 : Cannot allocate memory.\n"));
        exit(1);
    }
    
    childs->size = init_people;
    childs->used = 0;
    pinfo_emptyfill(childs);
    
    stats->array = malloc(sizeof(pinfo_t)*init_people);
    
    if(!stats)
    {
        perror(ERR("\nERROR #1 : Cannot allocate memory.\n"));
        exit(1);
    }
    
    stats->size = init_people;
    stats->used = 0;
    pinfo_emptyfill(stats);
    
    a_stats->array = malloc(sizeof(pinfo_t)*init_people);
    
    if(!a_stats)
    {
        perror(ERR("\nERROR #1 : Cannot allocate memory.\n"));
        exit(1);
    }
    
    a_stats->size = init_people;
    a_stats->used = 0;
    pinfo_emptyfill(a_stats);
    
    lengths = malloc(sizeof(int)*2);
    
    if(!lengths)
    {
        perror(ERR("\nERROR #1 : Cannot allocate memory. (a_stats)\n"));
        exit(1);
    }
    
    dims_shm_id = shmget(IPC_PRIVATE, sizeof(int)*2, 0600);
    lengths = shmat(dims_shm_id, 0, 0); // check!
    
    set_alarm();
    
    sync_start = getsem(init_people);
    gen_population();
    
    lengths[0] = a_stats->size;
    lengths[1] = init_people - lengths[0];
    
    /*
     * and so, god created death
     * well, not exactly into the bible, but who cares,
     * we're god in this universe.
     */
    create_death();
    
    for(i = 0; i < childs->size; i++)
    {
        termlistener(childs->array[i].pid);
    }
    
    return 0;
}

void sighandler(int signum)
{
    int i, last_killed = 0;
    // Quit the simulation and report activity
    switch(signum)
    {
        case SIGINT:
#ifdef VERBOSE
            printf(LOG("SIGINT received\n"));
#endif
            
            sim_sum();
            
            for (i = 0; i < childs->size; i++)
            {
                // inform processes to terminate

                kill(childs->array[i].pid, SIGTERM);
            }
            
            kill(timer, SIGTERM);
            
            // print: #processes of type lived in simulation

            printf("Individual with longest name:\n");
            print_proc(greatest_name(stats));
            
            printf("Individual with longest genome:\n");
            print_proc(greatest_genome(stats));
            
            
            // remove message queue
            msgctl(child_queue , IPC_RMID , NULL);
            
            // remove shared memory
            rem_shmem(shmem_id);

            rem_shmem(a_arr_id);
            
            // remove semaphores
            destroy(sync_start);

            // remove allocs
            
            pinfo_arr_free(childs);
            
            // Just if called SIGTERM before.
            pinfo_arr_free(childs);
            break;
            
        case SIGALRM:
#ifdef VERBOSE
            printf(LOG("SIGALRM received\n"));
#endif
            
            sim_sum();
            
            for (i = 0; i < childs->size; i++)
            {
                // inform processes to terminate

                kill(childs->array[i].pid, SIGTERM);
            }
            
            kill(timer, SIGTERM);
            
            // print: #processes of type lived in simulation

            printf("Individual with longest name:\n");
            print_proc(greatest_name(stats));
            
            printf("Individual with longest genome:\n");
            print_proc(greatest_genome(stats));
            
            
            // remove message queue
            msgctl(child_queue , IPC_RMID , NULL);
            
            // remove shared memory
            rem_shmem(shmem_id);

            rem_shmem(a_arr_id);
            
            // remove semaphores
            destroy(sync_start);

            // remove allocs
            
            pinfo_arr_free(childs);
            
            // Just if called SIGTERM before.
            pinfo_arr_free(childs);
            break;
            
        case SIGUSR1:

            last_killed = rand() % childs->size+1;

            kill(childs->array[last_killed].pid, SIGTERM);
            printf("killing: %d\n", last_killed);
            
#ifdef VERBOSE
            printf(CYAN("[manager|%d]: Hi mr. %s, your time has come.\n"), getpid(), childs->array[last_killed].name);
            print_proc(childs->array[last_killed]);
#endif
            // Clear the information.
            if (childs->array[last_killed].type == 'A')
                empty(&a_stats->array[contains(childs->array[last_killed], a_stats)]);
            
            empty(&childs->array[last_killed]);
            
            // after killing one, wait for him.
            termlistener(childs->array[insert(gen_child(NULL, 2), childs)].pid);
            break;
    }
}

void set_alarm()
{
    // Function variables
    struct sigaction sa;
    sigset_t  my_mask;
    
    // Starting by sending an alarm to self
    alarm(sim_time);
    
    // Initialize the sigaction
    sa.sa_handler = &sighandler;
    sa.sa_flags = 0;
    
    sigemptyset(&my_mask);        // Do not mask any signal
    sa.sa_mask = my_mask;
    
    sigaction(SIGALRM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
}

void sim_sum(void)
{
    printf("\nSimulation report:\n");
    printf("- %d processes\n", stats->size);
    printf("- %d A processes\n", a_stats->used);
    printf("- %d B processes\n", stats->used - a_stats->used);
    
    printf("----------------\n");
    
    printf("\nContents of stats:\n");
    print_arr(stats);
    printf("\n\n");
    
    printf("\nContents of childs:\n");
    print_arr(childs);
    printf("\n\n");
    
    printf("\n\nContents of A processes stats:\n");
    print_arr(a_stats);
    printf("\n\n");
}

void grant_diversity(pinfo_t *member, pinfo_arr_t *community)
{
	/*
	 * Diversity: Ensure the population isn't formed by same-type individuals.
	 */
    int i, need_to_change = 1;
    char tmpch;

    // 1: check the population is effectively formed by same-type individuals
    tmpch = community->array[0].type;
    for (i = 0; i < community->size; i++)
    {
        //type correction
        while (!is_empty(community->array[i]) && 
        	community->array[i].type == member->type)
        {
            need_to_change = 0;
            break;
        }
    }
    if (need_to_change)
    {
        member->type = member->type == 'A' ? 'B' : 'A';
    }
    else
    {
        return;
    }
}

char gen_type()
{
    int random = 0;
    random = rand() % 2 + 'A';
    return (char) random;
}

char* gen_name(char* old)
{
    char *random;
    
    if(old != NULL)
    {
        random = malloc(sizeof(old)+ sizeof(char));
        sprintf(random, "%s", old);
        random[strlen(old)] = rand() % 26 + 'A';
        random[strlen(old)+1] = '\0';
    }
    else
    {
        random = malloc(sizeof(char)*2);
        sprintf(random, "%c", rand() % 26 + 'A');
    }
    return random;
}

unsigned long gen_genome(unsigned long x)
{
    unsigned long random = 0;
    x = x == 0 ? 2 : x;
    random = rand() % ((x+genes)-x) + x;
    return random;
}

pinfo_t gen_child(char* o_name, unsigned long xseed)
{
    pinfo_t child;
    char *arguments[9];
    char *envp[] = { (char*)0 };
    char tmpname[];
    
    /*
     * Three buffers to convert parameters to strings
     */
    char type_buf [2] = {0}, genome_buf [32] = {0}, shmid_buf [32] = {0},
    	 cqid_buf [32] = {0}, len_buf [32] = {0}, a_arr_buf [32] = {0};
    
    // In the parent now we have all the arguments.
    // fork(): now we have init_people processes with the same code
    
    child.type = gen_type();
    child.genome = gen_genome(xseed);
    if (o_name == NULL)
    {
        child.type = gen_type();
        tmpname = gen_name(NULL)
        strcpy(child.name, tmpname);
        child.genome = gen_genome(0);
        free(tmpname);
    }
    else
    {
        child.type = gen_type();
        tmpname = gen_name(o_name)
        strcpy(child.name, tmpname);
        child.genome = gen_genome(xseed);
        free(tmpname);
    }
    strcpy(child.name, tmpname);
    free(tmpname);    
    
    fflush(stdout); // make sure parent and child don't try to print strange things
    child.pid = fork();
    
    // check if we are the parent
    if(child.pid != 0)
    {
        grant_diversity(&child, childs);
        if (insert(child, stats) == -1) printf("error inserting into stats");

        if (child.type == 'A')
        	if (insert(child, a_stats) == -1) printf("error inserting into As");
    }
    
    // check who we are
    switch(child.pid)
    {
        case -1:
            perror(ERR("\nERROR #4 : Cannot fork\n"));
            exit(4);
            break;
            
        case 0:
            // we are  the child.
            /*
             * Name of the executable to be passed.
             * also, that should be the 1st argument
             * passed to the process program, so we
             * pass it within the arguments
             */
            arguments[0] = "./process.bin";
            
            /*
             * Convert chars and unsigned longs to strings so we can pass them through
             * execve arguments.
             */

            sprintf(type_buf, "%c", child.type);
            arguments[1] = type_buf;
            
            arguments[2] = child.name;
            
            sprintf(genome_buf, "%lu", child.genome);
            arguments[3] = genome_buf;
            
            sprintf(shmid_buf, "%d", shmem_id);
            arguments[4] = shmid_buf;
            
            sprintf(cqid_buf, "%d", child_queue);
            arguments[5] = cqid_buf;
            
            sprintf(len_buf, "%d", dims_shm_id);
            arguments[6] = len_buf;
            
            sprintf(a_arr_buf, "%d", a_arr_id);
            arguments[7] = a_arr_buf;
            
            arguments[8] = NULL;
            
            if (o_name == NULL && xseed == 0)
            {
                acquire(sync_start);
                execve(arguments[0], arguments, envp);
                release(sync_start);
            }
            else
            {
                execve(arguments[0], arguments, envp);
            }
            
            break;
    }
    return child;
}

void gen_population()
{
    int i;
    
    for(i = 0; i < init_people; i++)
    {
        if (insert(gen_child(NULL, 0), childs) == -1) perror("Error inserting into childs");
    }
}

void create_death()
{
    char *arguments[3];
    char *envp[] = { (char*)0 };
    char buf[8];
    memset(buf, 0, sizeof(buf));

    // kill randomly someone, 'cause we can
    timer = fork();
    
    switch(timer)
    {
        case -1:
            perror(ERR("\nERROR #4 : Cannot fork\n"));
            exit(4);
            break;
            
        case 0:
            // we are the child
            
            arguments[0] = "timer.bin";
            sprintf(buf, "%d", birth_death);
            arguments[1] = buf;
            arguments[2] = NULL;
            
            execve(arguments[0], arguments, envp);
            break;
    }
}

void termlistener(pid_t chpid)
{
    pinfo_t chpair[2];
    int status, mret;
    pid_t terminated = 0, terminated_paired = 0;
    msgbuf_t mesg;

    // just listen for 'A' processes
    mret = searchby_pid(a_stats, chpid);
    if (mret != -1)
    {
        terminated = waitpid(chpid, &status, 0);
        if (terminated != -1)
        {
            mret = msgrcv(child_queue, &mesg, sizeof(msgbuf_t)- sizeof(long), terminated, 0);
            if (mret != -1)
            {
                pair[0] = mesg.subject;
                terminated_paired = waitpid(pair[0].pid, &status, 0);
                if (terminated_paired != -1)
                {
                    mret = msgrcv(child_queue, &mesg, sizeof(msgbuf_t)- sizeof(long), terminated_paired, 0);
                    if (mret != -1)
                    {
                        pair[1] = mesg.subject;
                        chpair[0] = gen_child(pair[0].name, calc_mcd(pair[0].genome, pair[1].genome));
                        chpair[1] = gen_child(pair[1].name, calc_mcd(pair[0].genome, pair[1].genome));

                        pinfo_remove(pair[0], childs);
                        pinfo_remove(pair[1], childs);
                        if (pair[0].type == 'A') pinfo_remove(pair[0], a_stats);
                        if (pair[1].type == 'A') pinfo_remove(pair[1], a_stats);

                        termlistener(chpair[0].pid);
                        termlistener(chpair[1].pid);
                    }
                }
            }
        }
    }
}

