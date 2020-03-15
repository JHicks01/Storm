#ifndef _HEAP_H_
#define _HEAP_H_

#include <stddef.h>

void heap_init(void);

void * heap_malloc(size_t size);

void heap_free(void *ptr);

#endif /* _HEAP_H_ */
