/*  Littlewood-Richardson Calculator
 *  Copyright (C) 1999- Anders S. Buch (asbuch at math rutgers edu)
 *  See the file LICENSE for license information.
 */

#include <stdio.h>

#include <alloc.h>
#include <vector.h>
#include <hashtab.h>

#include "symfcn.h"
#include "ssymfcn.h"

long long lrcalc_lrcoef(vector *outer, vector *inner1, vector *inner2)
{
  if (setjmp(lrcalc_panic_frame) != 0)
    return -1;
  return lrcoef(outer, inner1, inner2);
}

hashtab *lrcalc_skew(vector *outer, vector *inner, int maxrows)
{
  if (setjmp(lrcalc_panic_frame) != 0)
    return NULL;
  return skew(outer, inner, maxrows);
}

hashtab *lrcalc_coprod(vector *part, int all)
{
  if (setjmp(lrcalc_panic_frame) != 0)
    return NULL;
  return coprod(part, all);
}

hashtab *lrcalc_mult(vector *sh1, vector *sh2, int maxrows)
{
  if (setjmp(lrcalc_panic_frame) != 0)
    return NULL;
  return mult(sh1, sh2, maxrows);
}

hashtab lrcalc_mult_quantum(vector *sh1, vector *sh2, 

hashtab lrcalc_mult_fusion(vector *sh1, vector *sh2, 
			   int rows, int cols, int opt_zero)
{
  hashtab *res;
  if (setjmp(lrcalc_panic_frame) != 0)
    return NULL;
  res = mult(sh1, sh2, rows);
  fusion_reduce(res, rows, cols, opt_zero);
  return res;
}

