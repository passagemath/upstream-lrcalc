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
  int i, j, lam0, lami, len, sign, q, n, dif;
  
  len = v_length(lambda);
  sign = 1;
  q = 0;
  n = rows + cols;
  
  while (len > 0 && v_elem(lambda, 0) > cols)
    {
      lam0 = v_elem(lambda, 0);
      i = len;
      lami = 0;
      while (i > 0 && lam0 - lami + i > n)
	lami = v_elem(lambda, --i);
      
      if (i == len)
	return 0;
      
      dif = n - lam0 + lami - i;
      if (dif == 0)
	return 0;
      
      for (j = 0; j < i; j++)
	v_elem(lambda, j) = v_elem(lambda, j + 1) - 1;
      
      v_elem(lambda, i) -= dif;
      
      while (len > 0 && v_elem(lambda, len - 1) == 0)
	len--;
      
      if (((rows + i + 1) & 1) != 0)
	sign = -sign;
      q++;
    }
  
  v_length(lambda) = len;
  *qp = q;
  return sign;
}


void print_usage()
{
  fprintf(stderr, "Usage: mult [-m] [-r rows] [-q rows,cols] part1 - part2\n");
  exit(1);
}


int main(int ac, char **av)
{
  hashtab *s;
  vector *sh1, *sh2;
  int c;
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
      maxq = (v_sum(sh1) + v_sum(sh2)) / n;
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
	  if (opt_maple)
	    maple_print_lincomb(tab, symbol, 0);
	  else
	    print_vec_lincomb(tab);
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
