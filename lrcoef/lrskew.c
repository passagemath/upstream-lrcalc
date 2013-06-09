/*  Littlewood-Richardson Calculator
 *  Copyright (C) 1999- Anders S. Buch (asbuch at math rutgers edu)
 *  See the file LICENSE for license information.
 */

#include <stdio.h>
#include <unistd.h>
extern char *optarg;

#include <vector.h>
#include <vectarg.h>

#include "symfcn.h"


void print_usage()
{
  fprintf(stderr, "usage: lrskew [-r rows] outer / inner\n");
  exit(1);
}

void print_lrskew_set(vector *outer, vector *inner, int opt_rows)
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
  
  st = st_new(out0, in0, NULL, opt_rows);
  do {
    st_print(st);
    putchar('\n');
  } while (st_next(st));

  st_free(st);
}


int main(int ac, char **av)
{
  int c;
  vector *outer, *inner;
  int opt_rows = 0;

  if (setjmp(lrcalc_panic_frame))
    {
      fprintf(stderr, "out of memory.\n");
      exit(1);
    }
  
  while ((c = getopt(ac, av, "mr:")) != EOF)
    switch (c)
      {
      case 'r':
	opt_rows = atoi(optarg);
	break;
      default:
	print_usage();
      }
  
  outer = get_vect_arg(ac, av);
  inner = get_vect_arg(ac, av);
  
  if (inner == NULL)
    print_usage();
  
  print_lrskew_set(outer, inner, opt_rows);
  
  v_free(outer);
  v_free(inner);
  
  memory_report;
  
  return 0;
}

