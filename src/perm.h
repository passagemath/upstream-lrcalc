#ifndef _PERM_H
#define _PERM_H

#include "ivector.h"
#include "ivlist.h"

#ifdef _PERM_C
#undef INLINE
#define INLINE CINLINE
#endif


INLINE int perm_valid(ivector *w)
{
  int n, i, a;
  n = iv_length(w);
  for (i = 0; i < n; i++)
    {
      a = abs(iv_elem(w, i)) - 1;
      if (a < 0 || a >= n || iv_elem(w, a) < 0)
        return 0;
      iv_elem(w, a) = - iv_elem(w, a);
    }
  for (i = 0; i < n; i++)
    iv_elem(w, i) = - iv_elem(w, i);
  return 1;
}

INLINE int perm_length(ivector *w)
{
  int i, j, n, res;
  n = iv_length(w);
  res = 0;
  for (i = 0; i < n-1; i++)
    for (j = i+1; j < n; j++)
      if (iv_elem(w, i) > iv_elem(w, j))
        res++;
  return res;
}

INLINE int perm_group(ivector *w)
{
  int i = iv_length(w);
  while (i > 1 && iv_elem(w, i-1) == i)
    i--;
  return i;
}

INLINE int dimvec_valid(ivector *dv)
{
  int i, ld = iv_length(dv);
  if (ld == 0)
    return 0;
  if (iv_elem(dv, 0) < 0)
    return 0;
  for (i = 1; i < ld; i++)
    if (iv_elem(dv, i-1) > iv_elem(dv, i))
      return 0;
  return 1;
}

int str_iscompat(ivector *str1, ivector *str2);

ivlist *all_strings(ivector *dimvec);
ivlist *all_perms(int n);

ivector *string2perm(ivector *str);
ivector *str2dimvec(ivector *str);
ivector *perm2string(ivector *perm, ivector *dimvec);

#endif
