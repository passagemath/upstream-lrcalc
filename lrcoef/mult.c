/*  Littlewood-Richardson Calculator
 *  Copyright (C) 1999 Anders S. Buch (abuch@math.mit.edu)
 *  See the file LICENSE for license information.
 */

#include <stdio.h>
#include <unistd.h>
extern char *optarg;

#include <vectarg.h>

#include "symfcn.h"
#include "maple.h"


void print_usage()
{
  fprintf(stderr, "Usage: mult [-m] [-r rows] part1 - part2\n");
  exit(1);
}


int main(int ac, char **av)
{
  hashtab *s;
  vector *sh1, *sh2;
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
  
  sh1 = get_vect_arg(ac, av);
  sh2 = get_vect_arg(ac, av);
  
  if (sh1 == NULL || sh2 == NULL)
    print_usage();
  
  s = mult(sh1, sh2, opt_rows);
  if (opt_maple)
    maple_print_lincomb(s, "s");
  else
    print_vec_lincomb(s);
  
#if 0
  hash_print_stat(s, 10);
#endif
  
  v_free(sh1);
  v_free(sh2);
  free_vec_lincomb(s);
  
  memory_report;
  
  return 0;
}
