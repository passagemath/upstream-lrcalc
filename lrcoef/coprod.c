/*  Littlewood-Richardson Calculator
 *  Copyright (C) 1999- Anders S. Buch (asbuch at math rutgers edu)
 *  See the file LICENSE for license information.
 */

#include <stdio.h>
#include <unistd.h>

#include <hashtab.h>
#include <vector.h>
#include <vectarg.h>

#include "symfcn.h"


void print_usage()
{
  fprintf(stderr, "usage: coprod [-a] part\n");
  exit(1);
}


int main(int ac, char **av)
{
  hashtab *s;
  vector *part;
  int c, opt_all = 0;
  
  if (setjmp(lrcalc_panic_frame))
    {
      fprintf(stderr, "out of memory.\n");
      exit(1);
    }
  
  while ((c = getopt(ac, av, "a")) != EOF)
    switch (c)
      {
      case 'a':
	opt_all = 1;
	break;
      default:
	print_usage();
      }
  
  part = get_vect_arg(ac, av);
  if (part == NULL)
    print_usage();
  
  s = coprod(part, opt_all);
  print_vp_lincomb(s);

  free_vp_lincomb(s);
  v_free(part);
  
  memory_report;
  
  return 0;
}

