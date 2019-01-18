/* xsh_hello.c */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include "shprototypes.h"
/*------------------------------------------------------------------------
 * hello command 
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int args, char *arg[])
{
  if (args != 2) 
    {
      printf("Must have one argument brotha!\n");
    }
  else
    {
  printf("Hello %s, Welcome to the world of Xinu!!\n",arg[1]);
    }

  /* printf("Hello %s, Welcome to the world of Xinu!!\n",arg[1]);*/
	return 0;
}
