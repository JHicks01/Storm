#ifndef _THREAD_H_
#define _THREAD_H_

#include <stddef.h>

struct thread;

typedef void (*thread_fn_t)(void *arg);

int thread_init(void);

int thread_create(struct thread **threadp,
                  const char *name,
                  thread_fn_t fn,
                  void *arg);

void thread_start_scheduler(void);

#endif /* _THREAD_H_ */
