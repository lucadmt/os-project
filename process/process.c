#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> // strtoul
#include <sys/ipc.h>
#include <sys/msg.h>
#include "../utils/utils.h"
#include "../manager.h"
#include "process.h"

#ifndef VERBOSE
# define VERBOSE
#endif

#ifndef _USE_GNU
# define _USE_GNU
#endif

extern int errno;

pinfo_t self, the_chosen_one;;
pinfo_arr_t *a_stats, *a_av, *exes;
int msgq_id, mgr_msg_id, *lengths;

int main(int argc, char *argv[])
{
    /*
     * argv is made up this way:
     * argv[0]: name of the executable = ./process.bin (constant)
     * argv[1]: type of the process
     * argv[2]: name of the process
     * argv[3]: genome of the process
     * argv[4]: shared memory id (of a processes)
     * argv[5]: manager's message queue
     * argv[6]: shared memory id (of the lengths of the population)
     * argv[7]: '\0'
     */
    
    atexit(cleanup);
    
    msgbuf_t mesg;
    int mret = 0;
    
    // passed arguments processing
    self.pid = getpid();
    self.type = argv[1][0];
    strcpy(self.name, argv[2]);
    self.genome = strtoul(argv[3], NULL, 10);
    
    // self-presentation
    printf("\n\nHi! I'm just born, here's my identity card:\n");
    print_proc(self);
    
    // each process knows exactly the queue of the parent process.
    mgr_msg_id = atoi(argv[5]);
    
    lengths = shmat(atoi(argv[6]), 0, 0);

    // do our job

    switch(self.type)
    {
        case 'A':

            exes = pinfo_arr_new(lengths[1]); 
            
            // create the message queue to let a B process comunicate.
            msgq_id = msgget(self.pid, 0600 | IPC_CREAT);
            
			// create the message queue to let a B process comunicate.
			msgq_id = msgget(self.pid, 0600 | IPC_CREAT);

			// while an 'A' process hasn't choosen someone to marry it should
			// not exit.
			do
			{
				// see if there are messages for me
				mret = msgrcv(msgq_id, &mesg, sizeof(msgbuf_t)-sizeof(long), 
							  1, 0);
				if (mret != -1) // if we had successfully took a message
				{
					the_chosen_one = mesg.subject;

					printf(CYAN("[%s|%c]: Received message from: (%s|%c)\n"), 
						self.name,
						self.type,
						the_chosen_one.name,
						the_chosen_one.type);

					// Analyze the sender and check if it could be ours.
					if (is_process_eligible(the_chosen_one, self, exes))
					{
						printf(CYAN("[%s|%c]: Accepting purpose from: (%s|%c)\n"), 
							self.name,
							self.type,
							the_chosen_one.name,
							the_chosen_one.type);

						mesg.msg_type = 2;
						mesg.subject = self;
						mret = msgsnd(msgq_id, &mesg, 
							sizeof(msgbuf_t)-sizeof(long), 0);
                        
                        if (mret == -1)
                        {
                            perror("Failed message sending");
                        }

						mesg.msg_type = self.pid;
						mesg.subject = the_chosen_one;
						mret = msgsnd(mgr_msg_id, &mesg, 
							sizeof(msgbuf_t)-sizeof(long), 0);
                        
                        if (mret == -1)
                        {
                            perror("Failed message sending");
                        }
                        
						exit(0);
					}
					else
					{
						printf(CYAN("[%s|%c]: Refusing purpose from: (%s|%c)\n"), 
							self.name,
							self.type,
							the_chosen_one.name,
							the_chosen_one.type);

						mesg.msg_type = 3;
						mesg.subject = self;
						mret = msgsnd(msgq_id, &mesg, 
							sizeof(msgbuf_t)-sizeof(long), 0);
                        printf("(btoexes)\n");
						insert(the_chosen_one, exes);
					}
				}
				else
				{
					perror("Error while getting message (A)");
				}
			} while (1);
			TELL(self.name, self.type, "done.\n")
        break;
            
        case 'B':
        	
        	a_stats = shmat(atoi(argv[4]), 0, 0);
        	a_stats->array = shmat(atoi(argv[7]), 0, 0);
        	
        	a_av = pinfo_arr_new(a_stats->size);
        	            
            do
            {
            	// reset availability
                if (a_av->used == 0)
                {
                    pinfo_arr_cpy(a_av, a_stats);
                }
                
                // choose a process
                the_chosen_one = choose_the_one(a_stats, self);

                printf(CYAN("[%s|%c]: I made my choice (%s|%c)\n"), self.name,
                        	self.type,
                        	the_chosen_one.name,
                        	the_chosen_one.type);
                
                // add self to the message queue
                do
                {
                    msgq_id = msgget(the_chosen_one.pid, 0600);
                } while(msgq_id == -1);
                
                mesg.msg_type = 1;
                mesg.subject = self;
                mret = msgsnd(msgq_id, &mesg, sizeof(msgbuf_t)-sizeof(long), 0);
                if (mret == -1) perror(RED("B mesg sending failed"));
                
                // select first message != 1
                mret = msgrcv(msgq_id, &mesg, sizeof(msgbuf_t)-sizeof(long), 
                			1, MSG_EXCEPT);
                if (mret == -1) perror(RED("B mesg reception failed"));

                switch(mesg.msg_type)
                {
                	case 2:
                		printf(CYAN("[%s|%c]: She accepted (%s|%c) :)\n"), self.name,
                        	self.type,
                        	the_chosen_one.name,
                        	the_chosen_one.type);
                    
	                    mesg.msg_type = self.pid;
	                    mesg.subject = the_chosen_one;
	                    msgsnd(mgr_msg_id, &mesg, sizeof(msgbuf_t)-sizeof(long), 0);
	                    exit(0);
	                break;

	                case 3:
	                	printf("[%s|%c]: She refused :/ (%s|%c)\n", self.name,
                        	self.type,
                        	the_chosen_one.name,
                        	the_chosen_one.type);
                        
                        // refused, remove availability
                        pinfo_remove(the_chosen_one, a_av);
                    break;
                }
            } while (1);
            TELL(self.name, self.type, "done.\n")
        break;
    }
    return 0;
}

pinfo_t choose_the_one(pinfo_arr_t *arr, pinfo_t me)
{
    int i, j = 0;
    unsigned long tmpmcd = 0, tmpmcd2 = 0;
    for(i = 0; i < arr->size; i++)
    {
        if (arr->array[i].pid != 0)
        {
            tmpmcd2 = calc_mcd(me.genome, arr->array[i].genome);
            
            tmpmcd = tmpmcd > tmpmcd2 ? tmpmcd : tmpmcd2;
            
            j = tmpmcd2 > tmpmcd ? j : i;
        }
    }
    return arr->array[j];
}

int is_process_eligible(pinfo_t process, pinfo_t self, pinfo_arr_t *exes)
{
	if (!is_empty(process) &&
		is_multiple(process.genome, self.genome)) return 1; // always yes
	else
	if (!is_empty(process) && 
		exes->used == exes->size &&
		process.genome > greatest_mcd(exes)) return 1;
	else

	/* Adaptability:
	 *	 if we denied all of our pretendents, accept (lower the target)
	 */
	if (!is_empty(process) && exes->size == lengths[1]) return 1;

	// if none of the conditions is matched, process isn't eligible.
	else return 0;
}

unsigned long greatest_mcd(pinfo_arr_t *subset)
{
    int i;
    unsigned long max;
    for (i = 0; i < subset->size; i++)
    {
        max = max > subset->array[i].genome ? subset->array[i].genome : max;
    }
    return max;
}

void cleanup()
{
    int r, pos;
    
    r = shmdt( (void*) lengths);
    if (r == -1)
    {
        perror(ERR("\nERROR #12 : can't detach shared memory from process.\n"));
        exit(12);
    }
    
    if (self.type == 'A')
    {
        pinfo_arr_free(exes);
    	r = msgctl(msgq_id, IPC_RMID, NULL);
    	if (r == -1)
    	{
        	perror(ERR("\nERROR #15 : can't deallocate message queue.\n"));
        	exit(15);
    	}
    }
    
    if (self.type == 'B')
    {
        pos = searchby_pid(a_stats, the_chosen_one.pid);
        empty(&a_stats->array[pos]);
        r = shmdt( (void*) a_stats);
        if (r == -1)
        {
            perror(ERR("\nERROR #12 : can't detach shared memory from process.\n"));
            exit(12);
        }
        r = shmdt( (void*) a_stats->array);
        if (r == -1)
        {
            perror(ERR("\nERROR #12 : can't detach shared memory from process.\n"));
            exit(12);
        }

        pinfo_arr_free(a_av);
    }
}
