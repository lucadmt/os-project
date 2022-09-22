#include <stdio.h>
#include "init.h"
#include "../utils/utils.h"

#ifndef VERBOSE
# define VERBOSE
#endif

int sim_time = 0, init_people = 0, genes = 0, birth_death = 0;

void sim_init()
{
  printf("\n\nHow many people in simulation? [2]: ");
  iinput(&init_people, 8);
  if (init_people < 2)
  {
    printf(ERR("Invalid number of people in simulation, default (2) will be used."));
    init_people = 2;
  }
  fflush(stdout); // Force buffer to complete the writing to stdout

  printf("\nFor how much time simulation be runnning? (15 or more seconds) [120]: ");
  iinput(&sim_time, 10);
  if (sim_time <= 0)
  {
    printf(ERR("Invalid simulation time, default (120) will be used."));
    sim_time = 120; // 120s == 2m
  }
  fflush(stdout); // Force buffer to complete the writing to stdout

  printf("\nWhat is the maximum lenght of the genome? [15]: ");
  iinput(&genes, 5);
  if (genes <= 0)
  {
    printf(ERR("Invalid genes number, default (15) will be used."));
    genes = 15;
  }
  fflush(stdout); // Force buffer to complete the writing to stdout

  printf("\nHow many seconds should pass before a process dies? (5 or more seconds) [12]: ");
  iinput(&birth_death, 5);
  if (birth_death < 5)
  {
    printf(ERR("Invalid number of seconds, default (12) will be used."));
  }
  fflush(stdout); // Force buffer to complete the writing to stdout

  // ... Other initialization stuff if needed.
}

// void sim_from_file(char* path)
// {
//   FILE* config = fopen("./config.cfg", "r");
//   char* prop;
//   while (fgetc(config) != EOF)
//   {
//     prop = get_property(config);
//     free(config);
//   }
// }

void init(int n, char* params)
{
  // Simulation will use default initialization if no argument are passed.
  switch(n)
  {
    case 1:
      sim_init();
    break;

    // case 2:
    //   sim_from_file(params[1]);
    // break;

    case 4:
      init_people = atoi(&params[1]);
      sim_time = atoi(&params[2]);
      genes = atoi(&params[3]);
      birth_death = atoi(&params[4]);
    break;
  }
}
