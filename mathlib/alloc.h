#ifndef _ALLOC_H
#define _ALLOC_H

#include <stdlib.h>

void *amalloc(int size);
void *acalloc(int num, int size);
void *arealloc(void *p, int size);

#ifdef DEBUG
#define DEBUG_MEMORY
#endif

#ifdef DEBUG_MEMORY
void mem_report();
void afree(void *);
#define memory_report	mem_report()
#else
#define memory_report
#define afree(p)	(free(p))
#endif

#endif
