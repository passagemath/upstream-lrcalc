/*  Littlewood-Richardson Calculator
 *  Copyright (C) 1999- Anders S. Buch (asbuch at math rutgers edu)
 *  See the file LICENSE for license information.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "alloc.h"
#include "part.h"
#include "schur.h"
#include "perm.h"
#include "ivlincomb.h"
#include "schublib.h"


#define PROGNAME "test_fusion"

void print_usage()
{
  fprintf(stderr, "usage: " PROGNAME " rows level\n");
  exit(1);
}

void out_of_memory()
{
  fprintf(stderr, PROGNAME ": out of memory.\n");
  alloc_report();
  exit(1);
}


int test_mult_fusion(ivector *sh1, ivector *sh2, int rows, int level)
{
  return 0;
}


int main(int ac, char **av)
{
  int rows, level;
  ivector *sh1, *sh2;
  part_iter itr1, itr2;
  
  alloc_getenv();

  if (ac != 3)
    print_usage();
  rows = atoi(av[1]);
  level = atoi(av[2]);
  if (rows < 0 || level < 0)
    print_usage();

  sh1 = iv_new(rows);
  if (sh1 == NULL)
    out_of_memory();
  sh2 = iv_new(rows);
  if (sh2 == NULL)
    {
      iv_free(sh2);
      out_of_memory();
    }

  pitr_box_first(&itr1, sh1, rows, level);
  for (; pitr_good(&itr1); pitr_next(&itr1))
    {
      pitr_box_first(&itr2, sh2, rows, level);
      for (; pitr_good(&itr2); pitr_next(&itr2))
        {
          /* FIXME: add/subtract number from all parts. */
          if (test_mult_fusion(sh1, sh2, rows, level) != 0)
            {
              iv_free(sh1);
              iv_free(sh2);
              out_of_memory();
            }
        }
    }

  puts("success");
  iv_free(sh1);
  iv_free(sh2);
  alloc_report();
  return 0;
}
