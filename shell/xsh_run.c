/*Filename: shell/xsh_run.c
Created by: Joshua Cannon
Created on: 20190129
Last Modified by: Joshua Cannon
Last Modified on: 
20190131 * Takes an argument to map to a function and run it
20190206 * Added prodcons for execution. Make threads not established yet

 */
#include <xinu.h>
#include <prodcons.h>
#include <stdlib.h>
#include <string.h>
#include <stream_proc.h>

shellcmd xsh_prodcons(int nargs, char *args[]);
void stream_proc(int nargs, char *args[]);

shellcmd xsh_run(int nargs, char *args[])
{
  // maybe create switch case for function call?

if ((nargs == 1) || (strncmp(args[1], "list", 5) == 0))
    {
      printf("function implemented");
      // printf("my_function_2\n");
      return OK;
    }

    /* This will go past "run" and pass the function/process name and its
     * arguments. */

    args++;
    nargs--;

    if(strncmp(args[0], "prodcons", 13) == 0) {
		  /* simply call the function */
      // 
xsh_prodcons(nargs, args);
    }

 if(strncmp(args[0], "stream_proc", 13) == 0) {
		  /* simply call the function */
      // 
void stream_proc(nargs, args);

       /* create a process with the function as an entry point. */

/*resume (create((void *)my_function_2, 4096, 20, "my_func_2", 2, nargs, args);
 */    }
    }
