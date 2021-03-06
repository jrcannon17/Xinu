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
#include <stream_proc.h> //#include <stream_input.h>

shellcmd xsh_prodcons(int nargs, char *args[]);
void example_function1(int nargs, char *args[]) {
        printf("In Example Function 1\n");
}
//void stream_proc(int nargs, char *args[]);
void future_test(int nargs, char *args[]);

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

if(strncmp(args[0], "stream_proc",12 ) == 0) {
		  /* simply call the function */
   stream_proc(nargs, args);
  //resume(create(stream_proc, 4096, 20, "stream_proc", 2, nargs, args));
 }
if(strncmp(args[0], "stream_proc_future",12 ) == 0) {
		  /* simply call the function */
   stream_proc_future(nargs, args);
  //resume(create(stream_proc, 4096, 20, "stream_proc", 2, nargs, args));
 }
if(strncmp(args[0], "future_test", 12) == 0) {
		  /* simply call the function */
  //void future_test(nargs,args);
  resume(create(future_test, 4096, 20, "future_test", 2, nargs, args));
   
 }
}
