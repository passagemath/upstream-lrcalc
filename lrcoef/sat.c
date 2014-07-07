/*  Littlewood-Richardson Calculator
 *  Copyright (C) 1999- Anders S. Buch (asbuch at math rutgers edu)
 *  See the file LICENSE for license information.
 */

#include <stdio.h>
#include <unistd.h>

#include <set.h>
#include <vector.h>

#include "symfcn.h"
#include "lrcalc_jump.h"

#define NUM_PRIMES	50

int primes[NUM_PRIMES] = {
  2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61,
  67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137,
  139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211,
  223, 227, 229};


int opt_verbose = 0;
int opt_count = 0;
int opt_flush = 0;


void print_usage()
{
  fprintf(stderr, "usage: lrconj [-cvf] [-s steps] number\n");
  exit(1);
}

int check_pair(vector *Npart, vector *Nspart)
{
  hashtab *skewlist[NUM_PRIMES];
  vector *part, *spart, *p0, *s0;
  vector *elt;
  skewtab *st;
  int dv, i, coef;
  int ok_cases;

  if (opt_verbose)
    {
      puts("----");
      v_print(Npart);
      printf(" / ");
      v_printnl(Nspart);
      putchar('\n');
    }
  
  ok_cases = 0;
  
  dv = i_gcd(v_gcd(Npart), v_gcd(Nspart));
  
  part = v_new(l_length(Npart));
  spart = v_new(l_length(Nspart));


  /* calculate skew sets for prime divisors in dv */
  
  for (i = 0; i < NUM_PRIMES; i++)
    {
      int p = primes[i];
      if (dv % p == 0)
	{
	  v_div(part, Npart, p);
	  v_div(spart, Nspart, p);

	  if (opt_verbose)
	    {
	      printf("For p = %d, calculating skew ", p);
	      v_print(part);
	      printf(" / ");
	      v_print(spart);
	      puts(" :");
	    }
	  
	  skewlist[i] = skew(part, spart, 0);
	  
	  if (opt_verbose)
	    {
	      print_vec_lincomb(skewlist[i], 0);
	      putchar('\n');
	    }
	}
      else
	{
	  skewlist[i] = NULL;
	}
    }
  
  if (opt_verbose)
    {
      printf("Checking ");
      v_print(Npart);
      printf(" / ");
      v_printnl(Nspart);
      putchar('\n');
    }
  
  /* now go through skew set for Npart */
  
  elt = v_new(l_length(Npart));
  
  p0 = v_new_copy(Npart);
  s0 = v_new_zero(v_length(p0));
  for (i = 0; i < v_length(Nspart); i++)
    v_elem(s0, i) = v_elem(Nspart, i);
  st = st_new(p0, s0, NULL, 0);
  
  do {
    int sdv = v_gcd(st->conts);
    int i, clen;
    
    clen = v_length(st->conts);
    while (clen > 0 && v_elem(st->conts, clen - 1) == 0)
      clen--;
    
    if (opt_verbose)
      {
	int slen = v_length(st->conts);
	v_length(st->conts) = clen;
	printf("  ");
	v_printnl(st->conts);
	v_length(st->conts) = slen;
      }
    
    for (i = 0; i < NUM_PRIMES; i++)
      if (skewlist[i] != NULL && sdv % primes[i] == 0)
	{
	  int p = primes[i];
	  
	  if (opt_verbose)
	    printf("    p = %d: ", p);
	  
	  v_length(elt) = v_length(st->conts);
	  v_div(elt, st->conts, p);
	  v_length(elt) = clen;
	  
	  if (opt_verbose)
	    v_printnl(elt);
	  
	  coef = hash_lookupint(skewlist[i], elt);

	  if (coef <= 0)
	    {
	      printf("\nFAIL: p = %d\n", p);
	      if (coef == 0)
		puts("Saturation conjecture fails.");
	      else
		puts("One-if-one conjecture fails.");

	      printf("part = ");
	      v_printnl(Npart);
	      printf("sub1 = ");
	      v_printnl(Nspart);
	      printf("sub2 = ");
	      v_printnl(st->conts);

	      exit(0);
	    }
	  else
	    {
	      ok_cases++;

	      if (coef == 1)
		{
		  /* fail if contents repeats */
		  hash_insertint(skewlist[i], elt, -1);
                }
	    }
	}
    
    if (opt_verbose)
      putchar('\n');
    
  } while (st_next(st));
  
  v_free(elt);
  st_free(st);
  
  for (i = 0; i < NUM_PRIMES; i++)
    if (skewlist[i] != NULL)
      free_vec_lincomb(skewlist[i]);
  
  v_free(part);
  v_free(spart);

  return ok_cases;
}

int main(int ac, char **av)
{
  int M;
  set *partset, *spset;
  vector *part, *spart, *Npart, *Nspart;
  set_itr itr, itr2;
  int i, index, start_index = 0;
  char c;
  extern int optind;
  extern char *optarg;
  
  if (setjmp(lrcalc_panic_frame))
    {
      fprintf(stderr, "out of memory.\n");
      exit(1);
    }

  while ((c = getopt(ac, av, "cvfs:")) != EOF)
    switch (c)
      {
      case 'c':
	opt_count = 1;
	break;
      case 'v':
	opt_verbose = 1;
	break;
      case 'f':
	opt_flush = 1;
	break;
      case 's':
	start_index = atoi(optarg);
	break;
      default:
	print_usage();
      }
  
  if (optind == ac)
    print_usage();
  M = atoi(av[optind]);
  if (M <= 0)
    print_usage();
  
  
  /* find all interesting partitions of weight M */
  
  partset = s_new((cmp_t) v_cmp, (hash_t) v_hash);
  
  part = v_new(M);
  Npart = v_new(M);
  
  for (i = 0; i < NUM_PRIMES; i++)
    {
      int p = primes[i];
      
      if (M % p == 0)
	{
	  int sz = M / p;
	  
	  v_length(part) = 1;
	  v_elem(part, 0) = sz;
	  
	  do {
	    v_length(Npart) = v_length(part);
	    v_mult(Npart, p, part);
	    
	    if (! s_has(partset, Npart))
	      {
		s_insert(partset, v_new_copy(Npart));
	      }
	  } while (part_itr_sz(part));
	}
    }
  
  if (opt_count)
    {
      printf("%d\n", s_card(partset));
      exit(0);
    }
  
  if (opt_verbose)
    {
      printf("Interesting partitions of weight %d (%d):\n",
	     M, s_card(partset));
      print_vec_set(partset);
      puts("--------------------------------------------------------");
    }  
  
  
  /* test partitions of weight M one by one */
  
  spart = Npart;
  Nspart = v_new(M);
  
  spset = s_new((cmp_t) v_cmp, (hash_t) v_hash);
  
  for (Npart = s_first(partset, itr), index = 0; 
       Npart != NULL; 
       Npart = s_next(itr), index++)
    {
      int dv;
      vector * _Nspart;
      int ok_cas = 0;
      
      if (index < start_index)
	{
	  v_free(Npart);
	  continue;
	}
      
      if (opt_verbose)
	{
	  printf("Partition %d: ", index);
	}
      else
	{
	  printf("%d: ", index);
	}
      v_print(Npart);
      if (opt_verbose)
	{
	  putchar('\n');
	  putchar('\n');
	}
      
      /* first find all interesting sub-partitions of Npart */
      
      s_reset(spset);
      
      dv = v_gcd(Npart);
      for (i = 0; i < NUM_PRIMES; i++)
	{
	  int p = primes[i];
	  if (dv % p == 0)
	    {
	      int sz = M / p;
	      
	      v_length(part) = v_length(Npart);
	      v_div(part, Npart, p);
	      
	      v_length(spart) = v_length(part);
	      v_copy(spart, part);
	      
	      while (part_itr_sub(spart, part))
		{
		  if (2 * v_sum(spart) >= sz)
		    {
		      v_length(Nspart) = v_length(spart);
		      v_mult(Nspart, p, spart);
		      
		      if (! s_has(spset, Nspart))
			{
			  s_insert(spset, v_new_copy(Nspart));
			}
		    }
		}
	    }
	}
      
      if (opt_verbose) 
	{
	  printf("Interesting subpartitions (%d):\n", s_card(spset));
	  print_vec_set(spset);
	  putchar('\n');
	}
      
      /* now test all all combinations  Npart, Nspart */
      
      for (_Nspart = s_first(spset, itr2); _Nspart != NULL; 
	   _Nspart = s_next(itr2))
	{
	  ok_cas += check_pair(Npart, _Nspart);
	  
	  v_free(_Nspart);
	}
      
      v_free(Npart);

      if (! opt_verbose)   
	{
	  printf("  ok (%d cases)\n", ok_cas);
	}
      else
	{
	  printf("ok (%d cases)\n", ok_cas);
	  puts("--------------------------------------------------------");
	}
      
      if (opt_flush)
	fflush(stdout);
    }
  
  v_free(part);
  v_free(spart);
  v_free(Nspart);
  
  s_free(spset);
  s_free(partset);

  memory_report;
  
  return 0;
}
