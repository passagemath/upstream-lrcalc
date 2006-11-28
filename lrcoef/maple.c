/*  Littlewood-Richardson Calculator
 *  Copyright (C) 1999 Anders S. Buch (abuch@math.mit.edu)
 *  See the file LICENSE for license information.
 */

#include <stdio.h>
#include <vector.h>
#include <hashtab.h>
#include "maple.h"

#define OUTPUT_WIDTH 60

int _intlen(int x)
{
  int res = 1;
  if (x < 0) { x = -x; res++; }
  if (x >= 10) res++;
  if (x >= 100) res++;
  if (x >= 1000) res++;
  if (x >= 10000) res++;
  if (x >= 100000) res++;
  return res;
}

int maple_print_term(int c, vector *v, char *letter)
{
  int x, i;
#ifdef MULTLINE
  int cols;
#endif
  
  putchar((c < 0) ? '-' : '+');
  c = abs(c);
  printf("%d*%s[", c, letter);
#ifdef MULTLINE
  cols = _intlen(c) + strlen(letter) + 3 + v_length(v);
#endif
  
  for (i = 0; i < v_length(v); i++)
    {
      if (i > 0)
	putchar(',');
      x = v_elem(v, i);
      printf("%d", x);
#ifdef MULTLINE
      cols += _intlen(x);
#endif
    }
  putchar(']');
#ifdef MULTLINE
  return cols;
#else
  return 0;
#endif
}

void maple_print_lincomb(hashtab *ht, char *letter, int nl)
{
  hash_itr itr;
  int column;
  
#ifdef MULTILINE
  putchar('(');
#endif
  column = 1;
  for (hash_first(ht, itr); hash_good(itr); hash_next(itr))
    {
      if (hash_intvalue(itr) == 0)
	continue;
      
      column += maple_print_term(hash_intvalue(itr), hash_key(itr),letter);
#ifdef MULTILINE
      if (column >= OUTPUT_WIDTH)
	{
	  putchar('\n');
	  column = 0;
	}
#endif
    }
#ifdef MULTILINE
  printf(");\n");
#else
  if (nl)
    putchar('\n');
#endif
}
