/*  Littlewood-Richardson Calculator
 *  Copyright (C) 1999 Anders S. Buch (abuch@math.mit.edu)
 *  See the file LICENSE for license information.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alloc.h"

#if 0
#define DEBUG_MEMORY_PRINT
#endif
#if 1
#define DEBUG_PTR_REF
#endif


#ifndef DEBUG_MEMORY

void out_of_memory()
{
  fprintf(stderr, "out of memory.\n");
  exit(1);
}

void *amalloc(int size)
{
  void *p = malloc(size);
  if (p == NULL)
    out_of_memory();
  return p;
}

void *acalloc(int size, int num)
{
  void *p = calloc(size, num);
  if (p == NULL)
    out_of_memory();
  return p;
}

void *arealloc(void *p, int size)
{
  p = realloc(p, size);
  if (p == NULL)
    out_of_memory();
  return p;
}

#else  DEBUG_MEMORY

int memory_used = 0;

void out_of_memory()
{
  fprintf(stderr, "out of memory.\n");
  fprintf(stderr, "Memory balance: %d\n", memory_used);
  exit(1);
}

#define ALIGN 16

#define HEAD_SPACE	3
#define TAIL_SPACE	3

#ifdef DEBUG_PTR_REF
#define ADD_TO_SIZE	((HEAD_SPACE + TAIL_SPACE) * ALIGN)
#define ADD_TO_PTR	(HEAD_SPACE * ALIGN)
#else
#define ADD_TO_SIZE	ALIGN
#define ADD_TO_PTR	ALIGN
#endif


#ifdef DEBUG_PTR_REF

static void scramble_storage(void *p)
{
  int size = *((int *) p);
  memset(p + ADD_TO_PTR, 0x99, size);
}

static void init_storage(void *p)
{
  int i, size = *((int *) p);
  unsigned char *s = p;
  for (i = 4; i < ADD_TO_PTR; i++)
    s[i] = 0xa5;
  for (i = 0; i < TAIL_SPACE * ALIGN; i++)
    s[ADD_TO_PTR + size + i] = 0xa5;
}

static void check_storage(void *p)
{
  int i, size = *((int *) p);
  unsigned char *s = p;
  int dirty = 0, idx = 0;
  for (i = 4; i < ADD_TO_PTR; i++)
    if (s[i] != 0xa5)
      { dirty = 1; idx = i; }
  for (i = 0; i < TAIL_SPACE * ALIGN; i++)
    if (s[ADD_TO_PTR + size + i] != 0xa5)
      { dirty = 1; idx = ADD_TO_PTR + size + i; }
  if (dirty)
    {
      fprintf(stderr, "WARNING: Pointer 0x%08x dirty at index %d (0x%08x).\n",
	      (int) s, idx, (int) (s + idx));
    }
}

#endif


void *amalloc(int size)
{
  void *p = malloc(size + ADD_TO_SIZE);
#ifdef DEBUG_MEMORY_PRINT
  fprintf(stder, "malloc 0x%08x\n", (int) p);
#endif
  if (p == NULL)
    out_of_memory();
  memory_used += size;
  *((int *) p) = size;
#ifdef DEBUG_PTR_REF
  init_storage(p);
  scramble_storage(p);
#endif
  return ((char *) p) + ADD_TO_PTR;
}

void *acalloc(int num, int size)
{
  void *p = calloc(1, size * num + ADD_TO_SIZE);
#ifdef DEBUG_MEMORY_PRINT
  fprintf(stderr, "calloc 0x%08x\n", (int) p);
#endif
  if (p == NULL)
    out_of_memory();
  memory_used += size * num;
  *((int *) p) = size * num;
#ifdef DEBUG_PTR_REF
  init_storage(p);
#endif
  return ((char *) p) + ADD_TO_PTR;
}

void *arealloc(void *p, int size)
{
  ((char *) p) -= ADD_TO_PTR;
#ifdef DEBUG_MEMORY_PRINT
  fprintf(stderr, "realloc 0x%08x -> ", (int) p);
#endif
  p = realloc(p, size + ADD_TO_SIZE);
#ifdef DEBUG_MEMORY_PRINT
  fprintf(stderr, "0x%08x\n", (int) p);
#endif
  if (p == NULL)
    out_of_memory();
  memory_used += size - *((int *) p);
  *((int *) p) = size;
#ifdef DEBUG_PTR_REF
  init_storage(p);
  /* Could scramble new part here. */
#endif
  return ((char *) p) + ADD_TO_PTR;
}

void afree(void *p)
{
  int size;
  ((char *) p) -= ADD_TO_PTR;
#ifdef DEBUG_PTR_REF
  check_storage(p);
  scramble_storage(p);
#endif
  size = *((int *) p);
  memory_used -= size;
#ifdef DEBUG_MEMORY_PRINT
  fprintf(stderr, "free 0x%08x\n", (int) p);
#endif
  free(p);
}

void mem_report()
{
  fprintf(stderr, "Memory balance: %d\n", memory_used);
}

#endif DEBUG_MEMORY
