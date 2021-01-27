/*  Littlewood-Richardson Calculator
 *  Copyright (C) 1999- Anders S. Buch (asbuch at math rutgers edu)
 *  See the file LICENSE for license information.
 */

#include "alloc.h"
#include "ivector.h"
#include "ivlincomb.h"
#include "lriter.h"
#include "part.h"
#include "optshape.h"
#include "ivlist.h"
#include "ilist.h"

#define _SCHUR_C
#include "schur.h"


ivlincomb *schur_mult(ivector *sh1, ivector *sh2,
		      int rows, int cols, int partsz)
{
  skew_shape ss;
  ivlincomb *lc;
  if (optim_mult(&ss, sh1, sh2, rows, cols) != 0)
    return NULL;
  if (ss.sign)
    lc = lrit_expand(ss.outer, NULL, ss.cont, rows, cols, partsz);
  else
    lc = ivlc_new(5, 2);
  sksh_dealloc(&ss);
  return lc;
}


int fusion_reduce(ivector *la, int level, ivector *tmp)
{
  int rows, n, q, i, j, k, a, b, sign;

  rows = iv_length(la);
  n = rows + level;

  q = 0;
  for (i = 0; i < rows; i++)
    {
      a = iv_elem(la, i) + rows - i - 1;
      b = (a >= 0) ? (a / n) : -((n - 1 - a) / n);
      q += b;
      iv_elem(tmp, i) = a - b*n - rows + 1;
    }

  /* bubble sort */
  sign = (rows & 1) ? 0 : q;
  for (i = 0; i < rows - 1; i++)
    {
      k = i;
      a = iv_elem(tmp, k);
      for (j = i+1; j < rows; j++)
        if (a < iv_elem(tmp, j))
          {
            k = j;
            a = iv_elem(tmp, k);
          }
      if (k != i)
        {
          iv_elem(tmp, k) = iv_elem(tmp, i);
          iv_elem(tmp, i) = a;
          sign++;
        }
    }

  for (i = 0; i < rows; i++)
    {
      if (i > 0 && iv_elem(tmp, i-1) == iv_elem(tmp, i))
        return 0;
      k = i + q;
      a = iv_elem(tmp, i) + k + (k / rows) * level;
      iv_elem(la, k % rows) = a;
    }

  return (sign & 1) ? -1 : 1;
}


int fusion_reduce_lc(ivlincomb *lc, int rows, int level)
{
  ivlc_iter itr;
  ivlist *parts;
  ilist *coefs;
  ivector *sh, *tmp;
  int i, c, sign;

  tmp = iv_new(rows);
  if (tmp == NULL)
    return -1;

  /* Copy linear combination to lists. */
  parts = ivl_new(ivlc_card(lc));
  if (parts == NULL)
    {
      iv_free(tmp);
      return -1;
    }
  coefs = il_new(ivlc_card(lc));
  if (coefs == NULL)
    {
      iv_free(tmp);
      ivl_free(parts);
      return -1;
    }
  for (ivlc_first(lc, &itr); ivlc_good(&itr); ivlc_next(&itr))
    {
      ivl_append(parts, ivlc_key(&itr));
      il_append(coefs, ivlc_value(&itr));
    }
  ivlc_reset(lc);

  /* Reduce and reinsert terms. */
  for (i = 0; i < ivl_length(parts); i++)
    {
      sh = ivl_elem(parts, i);
      c = il_elem(coefs, i);
      sign = fusion_reduce(sh, level, tmp);
      if (ivlc_add_element(lc, sign * c, sh, iv_hash(sh),
                           LC_FREE_KEY | LC_FREE_ZERO) != 0)
        {
          iv_free(tmp);
          il_free(coefs);
          ivl_free_all(parts);
          return -1;
        }
    }
  iv_free(tmp);
  il_free(coefs);
  ivl_free(parts);
  return 0;
}


ivlincomb *schur_mult_fusion(ivector *sh1, ivector *sh2, int rows, int level)
{
  skew_shape ss;
  ivlincomb *lc;
  if (optim_fusion(&ss, sh1, sh2, rows, level) != 0)
    return NULL;
  if (ss.sign)
    lc = lrit_expand(ss.outer, NULL, ss.cont, rows, -1, rows);
  else
    lc = ivlc_new(5, 2);
  sksh_dealloc(&ss);
  if (lc == NULL)
    return NULL;

  if (fusion_reduce_lc(lc, rows, level) != 0)
    {
      ivlc_free_all(lc);
      return NULL;
    }
  return lc;
}


ivlincomb *schur_skew(ivector *outer, ivector *inner, int rows, int partsz)
{
  skew_shape ss;
  ivlincomb *lc;
  if (optim_skew(&ss, outer, inner, NULL, rows) != 0)
    return NULL;
  if (ss.sign)
    lc = lrit_expand(ss.outer, ss.inner, ss.cont, rows, -1, partsz);
  else
    lc = ivlc_new(5, 2);
  sksh_dealloc(&ss);
  return lc;
}


static inline
int _schur_coprod_isredundant(ivector *cont, int rows, int cols)
{
  int i, sz1, sz2;
  sz1 = - rows * cols;
  for (i = 0; i < rows; i++)
    sz1 += iv_elem(cont, i);
  sz2 = 0;
  for (i = rows; i < iv_length(cont); i++)
    sz2 += iv_elem(cont, i);
  if (sz1 != sz2)
    return (sz1 > sz2) ? 0 : 1;
  for (i = 0; i < rows; i++)
    {
      int df = iv_elem(cont, i) - cols - part_entry(cont, rows + i);
      if (df)
        return (df < 0) ? 0 : 1;
    }
  return 0;
}

static inline
ivlincomb *_schur_coprod_count(lrtab_iter *lrit, int rows, int cols)
{
  ivector *cont = lrit->cont;
  ivlincomb *lc = ivlc_new(IVLC_HASHTABLE_SZ, IVLC_ARRAY_SZ);
  if (lc == NULL)
    return NULL;
  for (; lrit_good(lrit); lrit_next(lrit))
    {
      if (_schur_coprod_isredundant(cont, rows, cols))
        continue;
      if (ivlc_add_element(lc, 1, cont, iv_hash(cont), LC_COPY_KEY) != 0)
        {
          ivlc_free_all(lc);
          return NULL;
        }
    }
  return lc;
}

static ivlincomb *_schur_coprod_expand(ivector *outer, ivector *content,
                               int rows, int cols, int partsz)
{
  lrtab_iter *lrit;
  ivlincomb *lc;
  lrit = lrit_new(outer, NULL, content, -1, -1, partsz);
  if (lrit == NULL)
    return NULL;
  lc = _schur_coprod_count(lrit, rows, cols);
  lrit_free(lrit);
  return lc;
}

ivlincomb *schur_coprod(ivector *sh, int rows, int cols, int partsz, int all)
{
  skew_shape ss;
  ivlincomb *lc;
  ivector *box;
  int i;

  box = iv_new(rows);
  if (box == NULL)
    return NULL;
  for (i = 0; i < rows; i++)
    iv_elem(box, i) = cols;

  if (all)
    {
      lc = schur_mult(sh, box, -1, -1, partsz);
      iv_free(box);
      return lc;
    }

  if (optim_mult(&ss, sh, box, -1, -1) != 0)
    {
      iv_free(box);
      return NULL;
    }

  lc = _schur_coprod_expand(ss.outer, ss.cont, rows, cols, partsz);
  sksh_dealloc(&ss);
  iv_free(box);
  return lc;
}


long long schur_lrcoef(ivector *outer, ivector *inner1, ivector *inner2)
{
  skew_shape ss;
  long long coef;
  if (optim_coef(&ss, outer, inner1, inner2) != 0)
    return -1;
  if (ss.sign <= 1)
    coef = ss.sign;
  else
    coef = lrit_lrcoef(ss.outer, ss.inner, ss.cont);
  sksh_dealloc(&ss);
  return coef;
}
