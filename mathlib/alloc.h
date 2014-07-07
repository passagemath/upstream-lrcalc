#ifndef _ALLOC_H
#define _ALLOC_H

#include <stdlib.h>

void *amalloc(size_t size);
void *acalloc(size_t num, size_t size);
void *arealloc(void *p, size_t size);

#ifdef DEBUG
#define DEBUG_MEMORY
#endif

#if defined(DEBUG_MEMORY) || defined(SAGE)
void afree(void *);
#else
#define afree(p)	(free(p))
#endif

#ifdef DEBUG_MEMORY
void mem_report();
#define memory_report	mem_report()
#else
#define memory_report
#endif

#endif
