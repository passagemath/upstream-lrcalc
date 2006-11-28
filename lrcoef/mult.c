/*  Littlewood-Richardson Calculator
 *  Copyright (C) 1999 Anders S. Buch (abuch@math.mit.edu)
 *  See the file LICENSE for license information.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
extern char *optarg;

#include <vectarg.h>

#include "symfcn.h"
#include "maple.h"


void print_usage()
{
  fprintf(stderr,
"Usage: mult [-mz] [-r rows] [-q rows,cols] [-f rows,level] part1 - part2\n");
  exit(1);
}


int main(int ac, char **av)
{
  hashtab *s;
  vector *sh1, *sh2;
  int c, wt1, wt2;
  int opt_maple = 0;
  int opt_zero = 0;
  int opt_rows = 0;
  int opt_cols = 0;
  int opt_quantum = 0;
  int opt_fusion = 0;
  char *p;
  
  while ((c = getopt(ac, av, "mzr:q:f:")) != EOF)
    switch (c)
      {
      case 'm':
	opt_maple = 1;
	break;
      case 'z':
	opt_zero = 1;
	break;
      case 'r':
	opt_rows = atoi(optarg);
	if (opt_rows <= 0)
	  print_usage();
	break;
      case 'q':
      case 'f':
	if (c == 'q')
	  opt_quantum = 1;
	else
	  opt_fusion = 1;
	opt_rows = strtol(optarg, &p, 10);
	if (p == NULL || *p != ',')
	  print_usage();
	opt_cols = atoi(p + 1);
	if (opt_rows <= 0 || opt_cols <= 0)
	  print_usage();
	break;
      default:
	print_usage();
      }
  
  sh1 = get_vect_arg(ac, av);
  sh2 = get_vect_arg(ac, av);

  if (sh1 == NULL || sh2 == NULL)
    print_usage();

  wt1 = v_sum(sh1);
  wt2 = v_sum(sh2);
  
  s = mult(sh1, sh2, opt_rows);
  
  if (opt_maple)
    printf("0");
  
  if (opt_quantum)
    {
      int n = opt_rows + opt_cols;
      list *qlist = quantum_reduce(s, opt_rows, opt_cols);  
      int i;
      
      for (i = 0; i < l_length(qlist); i++)
	{
	  hashtab *tab = l_elem(qlist, i);
	  char symbol[15];
	  sprintf(symbol, "q^%d*s", i);
	  
	  if (! opt_maple)
	    {
	      print_vec_lincomb(tab, opt_zero);
	    }
	  else if (wt1 + wt2 != n * i)
	    {
	      maple_print_lincomb(tab, symbol, 0);
	    }
	  else if (hash_card(tab) > 0)
	    {
	      hash_itr itr;
	      hash_first(tab, itr);
	      if (hash_intvalue(itr) != 0 || opt_zero)
		printf("%+d*q^%d", hash_intvalue(itr), i);
	    }
	  free_vec_lincomb(tab);
	}
      
      l_free(qlist);
      if (opt_maple)
	putchar('\n');
    }
  else
    {
      if (opt_fusion)
	fusion_reduce(s, opt_rows, opt_cols, opt_zero);
      
      if (opt_maple)
	maple_print_lincomb(s, "s", 1);
      else
	print_vec_lincomb(s, opt_zero);
      free_vec_lincomb(s);
    }

  v_free(sh1);
  v_free(sh2);
  
  memory_report;
  
  return 0;
}
