#include <stdio.h>
#include <stdlib.h>
#include "../utils/utils.h"
#include "../process/process.h"

pinfo_t choose_the_one(pinfo_arr_t *array, pinfo_t me);

int main()
{
	pinfo_t bob;
	pinfo_arr_t *bobbies;
	pinfo_arr_t *luls;

	bobbies = pinfo_arr_new(3);
	luls = pinfo_arr_new(bobbies->size);

	bob.pid = 1;
	bob.type = 'A';
	strcpy(bob.name, "BOB");
	bob.genome = 13;

	insert(bob, bobbies);

	bob.pid = 2;
	bob.type = 'B';
	strcpy(bob.name, "BOB");	
	bob.genome = 14;

	insert(bob, bobbies);

	bob.pid = 3;
	bob.type = 'A';
	strcpy(bob.name, "BOB");	
	bob.genome = 15;

	insert(bob, bobbies);

	printf("now overrunning\n");

	bob.pid = 4;
	bob.type = 'A';
	strcpy(bob.name, "BOB");	
	bob.genome = 16;

	insert(bob, bobbies);

	printf("that was legit, watch this:\n");

	bob.pid = 5;
	bob.type = 'A';
	strcpy(bob.name, "BOB");	
	bob.genome = 17;

	insert(bob, bobbies);

	printf("Now printing pinfo:\n");
	print_proc(bob);

	printf("Now printing array:\n");
	print_arr(bobbies);

	printf("Now removing element:\n");

	bob.pid = 2;
	bob.type = 'B';
	strcpy(bob.name, "BOB");	
	bob.genome = 14;

	print_proc(bob);

	print_proc(bobbies->array[pinfo_remove(bob, bobbies)]);

	printf("Now inserting a new one:\n");

	bob.pid = 42;
	bob.type = 'T';
	strcpy(bob.name, "EVIL");	
	bob.genome = 42;

	printf("inserted in: %d\n", insert(bob,bobbies));

	print_proc(bobbies->array[3]);

	printf("final bobbies array:\n");
	print_arr(bobbies);

	printf("------------------------------------------------\n");
	printf("Good, now testing copy:\n");
	pinfo_arr_cpy(luls, bobbies);
	print_arr(luls);
	printf("------------------------------------------------\n");

	printf("searching bob by pid:\n");
	print_proc(luls->array[searchby_pid(luls, bob.pid)]);

	bob.pid = 22;
	bob.type = 'D';
	strcpy(bob.name, "ALPHABOB");
	bob.genome = 48;

	insert(bob, luls);

	bob.pid = 24;
	bob.type = 'A';
	strcpy(bob.name, "EVIL");	
	bob.genome = 24;

	printf("choosing one:\n");
	print_proc(choose_the_one(luls, bob));

	pinfo_arr_free(bobbies);
	pinfo_arr_free(luls);

	return 0;
}

pinfo_t choose_the_one(pinfo_arr_t *arr, pinfo_t me)
{
    int i, j = 0;
    print_proc(me);
    print_arr(arr);
    //print_proc(arr->array[-1]);
    printf("size: %d, used: %d\n\n", arr->size, arr->used);
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