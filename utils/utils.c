#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>  // semget
#include <sys/ipc.h>    // semget
#include <sys/sem.h>    // semget
#include "utils.h"

#define VERBOSE

void iinput(int* ptr, int bufsize)
{
    int i = 0, j = 0;
    char buffer [bufsize];
    emptyfill(buffer, bufsize);
    
    fgets(buffer, sizeof(buffer), stdin);
    
    if (count_input(buffer, bufsize) > bufsize-2)
    {
        clearbuf();
    }
    
    for (; i < bufsize; i++)
    {
        if (isDigit(buffer[i]) && buffer[i] != '\n')
        {
            buffer[j] = buffer[i];
            j++;
        }
        else
        {
            buffer[j] = ' ';
        }
    }
    *ptr = atoi(buffer);
}

int isDigit(char c)
{
    if (c >= 48 && c <= 57)
        return 1;
    else
        return 0;
}

void clearbuf()
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF) { /* Discard unused user input*/ }
}

int count_input(char* buffer, int size)
{
    int c = 0, i = 0;
    for (; i <= size; i++)
    {
        if (buffer[i] != ' ' && buffer[i] != '\n' && buffer[i] != '\0')
            c += 1;
    }
    return c;
}

void emptyfill(char* buffer, int dimensions)
{
    int i = 0;
    for(; i <= dimensions; i++)
    {
        buffer[i] = ' ';
    }
}

void print_proc(pinfo_t proc)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("------------------\nPID: %d\nType: %c\nName: %s\nGenome: %lu\n------\
------------\n", proc.pid, proc.type, proc.name, proc.genome);
    setvbuf(stdout, NULL, _IOLBF, 0);
}

unsigned long calc_mcd(unsigned long x, unsigned long y)
{
    unsigned long z;
    while (x != 0)
    {
        z = x; x = y%x;  y = z;
    }
    return y;
}

int is_multiple(unsigned long x, unsigned long y)
{
    return x % y == 0;
}

pinfo_arr_t* pinfo_arr_new(int arrsize)
{
    pinfo_arr_t *arr = calloc(1, sizeof(pinfo_arr_t));
    if (!arr) return NULL;
    arr->size = arrsize;
    arr->used = -1;
    arr->array = calloc(arrsize+1, sizeof(pinfo_t));
    if (!arr->array) return NULL;
    return arr;
}

void pinfo_arr_free(pinfo_arr_t *arr)
{
    if (!arr) return;
    free(arr->array);
    free(arr);
}

int insert(pinfo_t member, pinfo_arr_t *subset)
{
    int inserted;
    pinfo_t undefined;
    undefined.pid = 0;
    if (!subset || is_empty(member)) return -1;
    if (subset->size < 0 || subset->used < -1 || subset->used > subset->size) return -1;
    
    if (subset->used == subset->size)
    {
        subset->size += 1;
        subset->array = realloc(subset->array, subset->size*sizeof(pinfo_t));
    }
    
    if (subset->used < subset->size)
    {
        inserted = contains(undefined, subset);
        subset->used +=1;
        subset->array[inserted] = member;
        return inserted;
    }
    else
    {
        perror("can't insert into pinfo_arr");
        return -1;
    }
}

int contains(pinfo_t member, pinfo_arr_t *subset)
{
    int i, contained = 0, ret = 0;
    for(i = 0; i < subset->size && contained == 0; i++)
    {
        contained = member.pid == subset->array[i].pid ? 1 : 0;
        ret = contained == 1 ? i : 0;
    }
    return ret;
}

int pinfo_remove(pinfo_t member, pinfo_arr_t *subset)
{
    int i = contains(member, subset);
    if (i != -1)
    {
        empty(&subset->array[i]);
        subset->used -= 1;
        return i;
    }
    else
    {
#ifdef VERBOSE
        printf(LOG("no such element to remove.\n"));
#endif
        return -1;
    }
}

int pinfo_arr_cpy(pinfo_arr_t *dest, pinfo_arr_t *src)
{
	int i;
	if (dest == NULL || src == NULL) return -1;

    if (src->size > dest->size)
    {
        dest->array = realloc(dest->array, src->size*sizeof(pinfo_t));
        dest->size = src->size;
        dest->used = 0;
    }

    pinfo_emptyfill(dest);

	for (i = 0; i < dest->size; i++)
	{
	    insert(src->array[i], dest);
	}
	dest->used = src->used;
	dest->size = src->size;
	return 0;
}

int is_empty(pinfo_t elem)
{
    return elem.pid == 0 && 
            elem.type == '\0' && 
            elem.name[0] == '\0' && 
            elem.genome == 0;
}

int pinfo_emptyfill(pinfo_arr_t *subset)
{
    int i;
    if (subset->size > 0)
    {
        for(i = 0; i < subset->size; i++)
        {
            empty(&subset->array[i]);
        }
        subset->used = 0;
        return 1;   // Operation successful
    }
    else
    {
        return -1;  // Error: no element in subset.
    }
}

pinfo_t greatest_name(pinfo_arr_t *subset)
{
    int i, pos = 0, that_one = 0;
    pinfo_t undefined;
    empty(&undefined);
    if (subset->used > 0)
    {
        for (i = 0; i < subset->size; i++)
        {
        	pos = strlen(subset->array[i].name) > that_one ? i : pos;
            that_one = strlen(subset->array[i].name) > that_one ? 
            strlen(subset->array[i].name) : that_one;
        }
    }
    else
    {
        return undefined;
    }
    return subset->array[pos];
}

pinfo_t greatest_genome(pinfo_arr_t *subset)
{
    int i, pos = 0;
    unsigned long that_one = 0;
    pinfo_t undefined;
    empty(&undefined);
    if (subset->used > 0)
    {
        for (i = 0; i < subset->size; i++)
        {
        	pos = subset->array[i].genome > that_one ? i : pos;
            that_one = subset->array[i].genome > that_one ? 
            subset->array[i].genome : that_one;
        }
    }
    else
    {
        return undefined;
    }
    return subset->array[pos];
}

int searchby_pid(pinfo_arr_t *subset, pid_t pid)
{
    int i, r = -1;
    if (subset == NULL || subset->used <= 0) return -1;
    for (i = 0; i < subset->size; i++)
    {
        if (subset->array[i].pid == pid)
        {
            r = i;
            break;
        }
    }
    return r;
}

void empty(pinfo_t *member)
{
    member->pid = 0;
    member->type = '\0';
    memset(member->name, 0, sizeof(member->name));
    member->genome = 0;
}

void print_arr(pinfo_arr_t *arr)
{
	int i;
	for(i = 0; i < arr->size; i++)
	{
		if (!is_empty(arr->array[i]))
			print_proc(arr->array[i]);
	}
}

// ----- Semaphore utilities -----

int getsem(int ival)
{
    int ret = semget(IPC_PRIVATE, 1, 0666);
    if(ret == -1)
    {
        perror(ERR("\nERROR #3 : Cannot get semaphore"));
        exit(3);
    }
    semctl(ret, 0, SETVAL, ival);
    return ret;
}

int acquire(int sid)
{
    int ret;
    struct sembuf ops[1];
    
    ops[0].sem_num = 0;
    ops[0].sem_op = -1;
    ops[0].sem_flg = 0;
    
    // Arrays are pointers to memory, so it's okay to specify ops without '&'.
    ret = semop(sid, ops, 1);
    
    if(ret != 0) {
        perror(ERR("\nERROR #5 : Error while acquiring semaphore"));
    }
    return ret;
}

int release(int sid)
{
    int ret;
    struct sembuf ops[1];
    
    ops[0].sem_num = 0;
    ops[0].sem_op = 1;
    ops[0].sem_flg = 0;
    
    ret = semop(sid, ops, 1);
    
    if(ret != 0) {
        perror(ERR("\nERROR #6 : Error while releasing semaphore"));
    }
    return ret;
}

int count(int sid)
{
    int ret = semctl(sid, 0, GETNCNT);
    return ret;
}

int destroy(int sid)
{
    int ret;
    
    ret = semctl(sid, 0, IPC_RMID);
    
    if(ret != 0) {
        printf(ERR("ERROR #7 : Error while removing semaphore no.: %d\n"), sid);
    }
    return ret;
}

// -------- Shared Memory utilities
int get_shmem(pinfo_arr_t *subset, int size)
{
    // Allocate memory for struct
    int ret = shmget(IPC_PRIVATE, sizeof(pinfo_arr_t) + sizeof(pinfo_t)*size*2, 0600);
#ifdef VERBOSE
    if(ret == -1)
    {
        PRINT_ERR
    }
#endif
    return ret;
}

int rem_shmem(int shid)
{
    int ret = shmctl(shid , IPC_RMID , NULL);
    if (ret != 0)
    {
        perror(ERR("ERROR #11 : Cannot remove shared memory\n"));
    }
    return ret;
}

// ---------- Message queue utils

