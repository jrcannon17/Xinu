#include<xinu.h>
#include<future.h>

pid32 get_process(future* f, int which_q) {
  pid32 newpid;

  if(which_q == 0){
    //set queues
    queue* t =f->set_queue;
    if(t == NULL) return -1;
    if (t->next == NULL) {
      f->set_queue = NULL;
      newpid = t->pid;
      freemem((char*)t, sizeof(queue));
      return newpid;
    }
    newpid = t->pid;
    f->set_queue = f->set_queue->next;
    freemem((char*)t, sizeof(queue));
    return newpid;
  } else {
    //get queues
    queue* t =f->get_queue;
    if(t == NULL) return -1;

    if (t->next == NULL) {
      f->get_queue = NULL;
      newpid = t->pid;
      freemem((char*)t, sizeof(queue));
      return newpid;
    }
    newpid = t->pid;
    f->get_queue = f->get_queue->next;
    freemem((char*)t, sizeof(queue));
    return newpid;
  }
  return -1;
}

void add_to_queue(future *f, int which_q) {
  queue* node = (queue*) getmem(sizeof(queue));
  node->next = NULL;
  node->pid = getpid();
  if (which_q == 0) {
    //set queue
    if(f->set_queue == NULL){
      f->set_queue = node;
    } else {
      queue *t = f->set_queue;
      while (t->next != NULL) {
        t = t->next;
      }
      t->next = node;
    }
  } else {
    //get queue
    if(f->get_queue == NULL){
      f->get_queue = node;
    } else {
      queue *t = f->get_queue;
      while (t->next != NULL) {
        t = t->next;
      }
      t->next = node;
    }
  }
}

void free_queue(queue* head) {
  while(head!=NULL){
    queue* q = head;
    head = head->next;
    freemem((char*) q, sizeof(queue));
  }
}
