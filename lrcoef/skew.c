/*  Littlewood-Richardson Calculator
 *  Copyright (C) 1999 Anders S. Buch (abuch@math.mit.edu)
 *  See the file LICENSE for license information.
 */

#include <stdio.h>
#include <unistd.h>
extern char *optarg;

#include <hashtab.h>
#include <vector.h>
#include <vectarg.h>

#include "symfcn.h"
#include "maple.h"


void print_usage()
{
  fprintf(stderr, "usage: skew [-m] [-r rows] outer / inner\n");
  exit(1);
}

int main(int ac, char **av)
{
  hashtab *s;
  vector *outer, *inner;
  int c;
  int opt_maple = 0;
  int opt_rows = 0;

  while ((c = getopt(ac, av, "mr:")) != EOF)
    switch (c)
      {
      case 'm':
	opt_maple = 1;
	break;
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
  
  s = skew(outer, inner, opt_rows);
  if (opt_maple)
    maple_print_lincomb(s, "s", 1);
  else
    print_vec_lincomb(s);
  
#if 0
  hash_print_stat(s, 10);
#endif
  
  v_free(outer);
  v_free(inner);
  free_vec_lincomb(s);
  
  memory_report;
  
  return 0;
}

