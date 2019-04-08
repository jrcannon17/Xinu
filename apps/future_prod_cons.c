#include<xinu.h>
#include<future.h>
#include <stdlib.h>

uint future_prod(future* fut,int n) {
  printf("Produced %d\n",n);
  future_set(fut, &n);
  return OK;
}

uint future_cons(future* fut) {
  int i, status;
  status = (int)future_get(fut, &i);
  if (status < 1) {
    printf("future_get failed\n");
    return -1;
  }
  printf("Consumed %d\n", i);
  return OK;
}
