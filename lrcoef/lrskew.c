/*  Littlewood-Richardson Calculator
 *  Copyright (C) 1999 Anders S. Buch (abuch@math.mit.edu)
 *  See the file LICENSE for license information.
 */

#include <stdio.h>

#include <vector.h>

#include "symfcn.h"


void print_usage()
{
  fprintf(stderr, "usage: skew outer / inner\n");
  exit(1);
}

void print_lrskew_set(vector *outer, vector *inner)
{
  vector *out0, *in0;
  skewtab *st;
  int n, i;
  
  n = v_length(outer);
  if (v_length(inner) > n)
    return;
  
  out0 = v_new_copy(outer);
  
  in0 = v_new_zero(n);
  for (i = 0; i < v_length(inner); i++)
    v_elem(in0, i) = v_elem(inner, i);
  
  if (! v_lesseq(in0, out0))
    {
      v_free(in0);
      v_free(out0);
    }
  
  st = st_new(out0, in0, NULL, 0);
  do {
    st_print(st);
    putchar('\n');
  } while (st_next(st));

  st_free(st);
}


int main(int ac, char **av)
{
  int n, i;
  vector *outer, *inner;

  i = 1;
  outer = v_new(ac);  
  n = 0;
  while (i < ac && *av[i] != '/')
    v_elem(outer, n++) = atoi(av[i++]);
  v_length(outer) = n;
  
  i++;
  inner = v_new(ac);
  n = 0;
  while (i < ac)
    v_elem(inner, n++) = atoi(av[i++]);
  v_length(inner) = n;
  
  print_lrskew_set(outer, inner);
  
  v_free(outer);
  v_free(inner);
  
  memory_report;
  
  return 0;
}

