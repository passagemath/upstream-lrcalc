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
  fprintf(stderr, "usage: lrcoef outer - inner1 - inner2\n");
  exit(1);
}

int main(int ac, char **av)
{
  vector *lm, *mu, *nu;
  
  nu = get_vect_arg(ac, av);
  lm = get_vect_arg(ac, av);
  mu = get_vect_arg(ac, av);
  
  if (mu == NULL)
    print_usage();
  
  printf("%lld\n", lrcoef(nu, lm, mu));
  
  v_free(lm);
  v_free(mu);
  v_free(nu);
  
  memory_report;
  
  return 0;
}
