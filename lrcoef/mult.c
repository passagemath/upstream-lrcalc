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


int rim_hook(vector *lambda, int rows, int cols, int *qp)
{
  int i, j, len, sign, q, n;
  
  len = v_length(lambda);
  n = rows + cols;

  q = 0;
  for (i = 0; i < len; i++)
    {
      int a = v_elem(lambda, i) + rows - i - 1;
      q += a / n;
      a %= n;
      v_elem(lambda, i) = a - rows + 1;
    }

  /* bubble sort :-( */
  sign = (rows & 1) ? 0 : q;
  for (i = 1; i < len; i++)
    {
      int a = v_elem(lambda, i);
      for (j = i; j > 0 && a > v_elem(lambda, j-1); j--)
	{
	  v_elem(lambda, j) = v_elem(lambda, j-1);
	}
      if (j > 0 && a == v_elem(lambda, j-1))
	return 0;
      v_elem(lambda, j) = a;
      sign += i - j;
    }
  
  for (i = 0; i < len; i++)
    {
      v_elem(lambda, i) += i;
      if (v_elem(lambda, i) < 0)
	return 0;
    }
  
  while (len > 0 && v_elem(lambda, len - 1) == 0)
    len--;
  v_length(lambda) = len;
  *qp = q;
  return (sign & 1) ? -1 : 1;
}


void print_usage()
{
  fprintf(stderr,
	  "Usage: mult [-m] [-r rows] [-q rows,cols] part1 - part2\n");
  exit(1);
}


int main(int ac, char **av)
{
  hashtab *s;
  vector *sh1, *sh2;
  int c, wt1, wt2;
  int opt_maple = 0;
  int opt_rows = 0;
  int opt_cols = 0;
  int opt_quantum = 0;
  char *p;
  
  while ((c = getopt(ac, av, "mr:q:")) != EOF)
    switch (c)
      {
      case 'm':
	opt_maple = 1;
	break;
      case 'r':
	opt_rows = atoi(optarg);
	if (opt_rows <= 0)
	  print_usage();
	break;
      case 'q':
	opt_quantum = 1;
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
  
  s = mult(sh1, sh2, opt_rows);
  
  if (opt_quantum)
    {
      int n, maxq, i, sign, q, *valuep;
      list *qlist;
      hashtab *tab;
      hash_itr itr;
      
      n = opt_cols + opt_rows;
      wt1 = v_sum(sh1);
      wt2 = v_sum(sh2);
      maxq = (wt1 + wt2) / n;
      qlist = l_newsz(maxq + 1);
      for (i = 0; i <= maxq; i++)
	l_append(qlist, hash_new((cmp_t) v_cmp, (hash_t) v_hash));
      
      for (hash_first(s, itr); hash_good(itr); hash_next(itr))
	{
	  vector *lambda = hash_key(itr);
	  int coef = hash_intvalue(itr);
	  
	  sign = rim_hook(lambda, opt_rows, opt_cols, &q);
	  
	  if (sign == 0)
	    {
	      v_free(lambda);
	      continue;
	    }
	  
	  tab = l_elem(qlist, q);
	  valuep = hash_mkfindint(tab, lambda);
	  *valuep += sign * coef;
	  if (! hash_key_used)
	    v_free(lambda);
	}
      
      for (i = 0; i <= maxq; i++)
	{
	  char symbol[15];
	  sprintf(symbol, "q^%d*s", i);
	  
	  tab = l_elem(qlist, i);
	  if (! opt_maple)
	    {
	      print_vec_lincomb(tab);
	    }
	  else if (wt1 + wt2 != n * i)
	    {
	      maple_print_lincomb(tab, symbol, 0);
	    }
	  else if (hash_card(tab) > 0)
	    {
	      hash_itr itr;
	      hash_first(tab, itr);
	      c = hash_intvalue(itr);
	      printf("%+d*q^%d", c, i);
	    }
	  free_vec_lincomb(tab);
	}
      
      hash_free(s);
      l_free(qlist);
      if (opt_maple)
	putchar('\n');
    }
  else
    {
      if (opt_maple)
	maple_print_lincomb(s, "s", 1);
      else
	print_vec_lincomb(s);
      free_vec_lincomb(s);
    }
  
#if 0
  hash_print_stat(s, 10);
#endif
  
  v_free(sh1);
  v_free(sh2);
  
  memory_report;
  
  return 0;
}
